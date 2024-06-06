#pragma once

#include <string>

#include <controller/commands/reflectors_controller_command.hpp>
#include <controller/commands/reflectors_controller_command_visitor.hpp>

namespace voltiris::controller {

class GoCommand : public ReflectorsControllerCommand {
public:
  inline explicit GoCommand(std::size_t reflector_index, double azimuth, double elevation)
      : _reflector_index(reflector_index), _azimuth(azimuth), _elevation(elevation) {}
  ~GoCommand() override = default;

  [[nodiscard]] std::size_t GetReflectorIndex() const { return _reflector_index; }
  [[nodiscard]] double GetAzimuth() const { return _azimuth; }
  [[nodiscard]] double GetElevation() const { return _elevation; }

private:
  inline void Visit(ReflectorsControllerCommandVisitor& visitor) override { visitor.ProcessCommand(*this); }

  std::size_t _reflector_index;
  double _azimuth;
  double _elevation;
};

}  // namespace voltiris::controller