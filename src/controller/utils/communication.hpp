#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

namespace voltiris::controller::utils {

constexpr std::size_t kBufferSize = 512;
constexpr std::chrono::milliseconds kDelayRead{200};
constexpr std::chrono::milliseconds kDelayWrite{50};

std::vector<std::string> GetAvailablePorts();

std::int32_t Connect(const std::string& port_name);

void Disconnect(std::int32_t port_handler);

bool Send(std::int32_t port_handler, const std::string& msg);

template <typename... Args>
bool Send(std::int32_t port_handler, const std::string& msg_fmt, Args... args) {
  char buffer[kBufferSize] = {'\0'};
  sprintf(buffer, msg_fmt.c_str(), args...);

  return Send(port_handler, buffer);
}

bool Read(std::int32_t port_handler, std::string& msg, const std::chrono::milliseconds& timeout = std::chrono::milliseconds(1000));

template <typename... Args>
std::int32_t Read(std::int32_t port_handler, const std::string& msg_fmt, Args... args) {
  std::string msg;

  if (!Read(port_handler, msg)) { return -1; }

  return sscanf(msg.c_str(), msg_fmt.c_str(), args...);
}

}  // namespace voltiris::controller::utils