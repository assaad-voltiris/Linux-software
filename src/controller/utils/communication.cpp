#include "communication.hpp"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

namespace voltiris::controller::utils {

namespace {

#define BAUDRATE B9600  // Baudrate (can be changed as needed)

}  // namespace

std::vector<std::string> GetAvailablePorts() {
  std::vector<std::string> port_names;

  std::filesystem::path p("/dev/");
  if (!exists(p)) { throw std::runtime_error(p.generic_string() + " does not exist"); }

  for (const auto& de : std::filesystem::directory_iterator(p)) {
    if (de.path().generic_string().find("ttyS") != std::string::npos) { port_names.push_back(de.path().generic_string()); }
  }

  std::sort(port_names.begin(), port_names.end());

  return port_names;
}

std::int32_t Connect(const std::string& port_name) {
  std::int32_t com_port = open(port_name.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

  if (com_port == -1) { throw std::runtime_error("Unable to open port."); }

  termios serial = {};
  serial.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
  serial.c_iflag = IGNPAR;
  serial.c_oflag = 0;
  serial.c_lflag = 0;
  tcflush(com_port, TCIFLUSH);
  tcsetattr(com_port, TCSANOW, &serial);

  return com_port;
}

void Disconnect(std::int32_t port_handler) { close(port_handler); }

bool Send(std::int32_t port_handler, const std::string& msg) {
  if (write(port_handler, msg.c_str(), msg.size()) == -1) { return false; }
  return true;
}

bool Read(std::int32_t port_handler, std::string& msg) {
  char receive_buffer[kBufferSize] = {'\0'};

  std::size_t bytes_read = read(port_handler, receive_buffer, kBufferSize - 1);
  if (bytes_read == 0) { return false; }

  receive_buffer[bytes_read] = '\0';

  return true;
}

}  // namespace voltiris::controller::utils