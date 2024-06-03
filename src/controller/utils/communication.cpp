#include "communication.hpp"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

#include <spdlog/spdlog.h>

namespace voltiris::controller::utils {

namespace {}  // namespace

std::vector<std::string> GetAvailablePorts() {
  std::vector<std::string> port_names;

  std::filesystem::path p("/dev/");
  if (!exists(p)) { throw std::runtime_error(p.generic_string() + " does not exist"); }

  for (const auto& de : std::filesystem::directory_iterator(p)) {
    auto path_str = de.path().generic_string();
    if (path_str.find("ttyUSB") != std::string::npos || path_str.find("ttyS") != std::string::npos || path_str.find("ttyACM") != std::string::npos) {
      port_names.push_back(de.path().generic_string());
    }
  }

  std::sort(port_names.begin(), port_names.end());

  return port_names;
}

std::int32_t Connect(const std::string& port_name) {
  std::int32_t com_port = open(port_name.c_str(), O_RDWR | O_NOCTTY);

  if (com_port == -1) { throw std::runtime_error("Unable to open port."); }

  termios serial = {};
  serial.c_cflag = CS8 | CLOCAL | CREAD;
  serial.c_iflag = IGNPAR;
  serial.c_oflag = 0;
  serial.c_lflag = 0;
  cfsetspeed(&serial, B9600);
  tcflush(com_port, TCIFLUSH);
  tcsetattr(com_port, TCSANOW, &serial);

  return com_port;
}

void Disconnect(std::int32_t port_handler) { close(port_handler); }

bool Send(std::int32_t port_handler, const std::string& msg) {
  spdlog::debug("Data to send: {}", msg);
  if (write(port_handler, msg.c_str(), msg.size()) == -1) { return false; }
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  return true;
}

bool Read(std::int32_t port_handler, std::string& msg, const std::chrono::milliseconds& timeout) {
  auto is_data_completed = [](const char* buffer, std::size_t end_index) { return buffer[end_index - 2] == '\r' && buffer[end_index - 1] == '\n'; };

  auto is_timeout_reached = [timeout](const auto& start_time) { return std::chrono::high_resolution_clock::now() - start_time >= timeout; };

  auto remove_n = [](const char* buffer, std::size_t begin_index, std::size_t end_index) {
    std::size_t block_start = begin_index, i = begin_index;
    while (i <= end_index) {
      if (buffer[i] == '\n' && i != end_index) {
        ++i;
      } else {
        if (block_start != i) {
          memmove((void*)&buffer[block_start], &buffer[i], end_index - i);

          end_index = end_index - i + block_start;
          memset((void*)&buffer[end_index], '\0', i - block_start);

          i = block_start;
        } else if (buffer[i] == '\r') {
          block_start += 2;
          i += 2;
        } else {
          ++block_start;
          ++i;
        }
      }
    }
  };

  auto start_time = std::chrono::high_resolution_clock::now();

  char receive_buffer[kBufferSize] = {'\0'};
  std::size_t receive_buffer_begin = 0;
  std::size_t receive_buffer_end = 0;

  while (!is_data_completed(receive_buffer, kBufferSize - receive_buffer_begin) && !is_timeout_reached(start_time)) {
    if (kBufferSize <= receive_buffer_end) { throw std::runtime_error("Receive buffer overflow."); }

    receive_buffer_end += read(port_handler, receive_buffer, kBufferSize - receive_buffer_begin);
    remove_n(receive_buffer, receive_buffer_begin, receive_buffer_end);
  }

  spdlog::debug("Received data: {}", receive_buffer);

  return is_data_completed(receive_buffer, kBufferSize - receive_buffer_begin);
}

}  // namespace voltiris::controller::utils