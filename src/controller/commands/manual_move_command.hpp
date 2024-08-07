#pragma once

#include <string>

#include <nlohmann/json.hpp>

#include <controller/commands/reflectors_controller_command.hpp>
#include <controller/commands/reflectors_controller_command_visitor.hpp>

namespace voltiris::controller {

class ManualMoveCommand : public ReflectorsControllerCommand {
public:
  enum class Direction { kUp, kDown, kEast, kWest };

  inline explicit ManualMoveCommand() = default;
  inline explicit ManualMoveCommand(Direction direction, double step, const std::vector<std::size_t>& reflectors)
      : _direction(direction), _step(step), _reflectors(reflectors) {}
  ~ManualMoveCommand() override = default;

  [[nodiscard]] Direction GetDirection() const { return _direction; }
  [[nodiscard]] double GetStep() const { return _step; }
  [[nodiscard]] const std::vector<std::size_t>& GetReflectors() const { return _reflectors; }

  [[nodiscard]] std::string ToJson() override;

private:
  inline void Visit(ReflectorsControllerCommandVisitor& visitor) override { visitor.ProcessCommand(*this); }

  Direction _direction = Direction::kUp;
  double _step = 0;
  std::vector<std::size_t> _reflectors;
};

void to_json(nlohmann::json& j, const ManualMoveCommand& command);

void from_json(const nlohmann::json& j, ManualMoveCommand& command);

}  // namespace voltiris::controller