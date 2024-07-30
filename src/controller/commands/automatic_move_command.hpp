#pragma once

#include <string>

#include <nlohmann/json.hpp>

#include <controller/commands/reflectors_controller_command.hpp>
#include <controller/commands/reflectors_controller_command_visitor.hpp>

namespace voltiris::controller {

class AutomaticMoveCommand : public ReflectorsControllerCommand {
public:
  inline explicit AutomaticMoveCommand() = default;
  inline explicit AutomaticMoveCommand(bool calibrate, bool theoretical_position, bool perp_to_sun, bool init_position)
      : _calibrate(calibrate), _theoretical_position(theoretical_position), _perp_to_sun(perp_to_sun), _init_position(init_position) {}
  ~AutomaticMoveCommand() override = default;

  [[nodiscard]] bool GetCalibrate() const { return _calibrate; }
  [[nodiscard]] bool GetTheoreticalPosition() const { return _theoretical_position; }
  [[nodiscard]] bool GetPerpToSun() const { return _perp_to_sun; }
  [[nodiscard]] bool GetInitPosition() const { return _init_position; }

  [[nodiscard]] std::string ToJson() override;

private:
  inline void Visit(ReflectorsControllerCommandVisitor& visitor) override { visitor.ProcessCommand(*this); }

  bool _calibrate;
  bool _theoretical_position;
  bool _perp_to_sun;
  bool _init_position;
};

void to_json(nlohmann::json& j, const AutomaticMoveCommand& command);

void from_json(const nlohmann::json& j, AutomaticMoveCommand& command);

}  // namespace voltiris::controller