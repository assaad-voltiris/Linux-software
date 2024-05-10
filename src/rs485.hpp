#pragma once

#include <vector>
#include <memory>

#include <arduino_mega.hpp>

namespace voltiris {

class RS485 {
public:
  explicit RS485() = default;
  ~RS485() = default;

  std::vector<ArduinoMega> _reflectors;
private:

};

} // namespace voltiris