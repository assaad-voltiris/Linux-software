#include "communication.hpp"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <stdexcept>

namespace voltiris::controller::utils {

namespace {
#define BAUDRATE B9600  // Baudrate (can be changed as needed)
}  // namespace

std::vector<std::string> GetAvailablePorts() { return {"/dev/ttyS5"}; }

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