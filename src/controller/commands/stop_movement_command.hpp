#pragma once

#include <string>

#include <nlohmann/json.hpp>

#include <controller/commands/reflectors_controller_command.hpp>
#include <controller/commands/reflectors_controller_command_visitor.hpp>

namespace voltiris::controller {

class StopMovementCommand : public ReflectorsControllerCommand {
public:
  inline explicit StopMovementCommand() = default;
  ~StopMovementCommand() override = default;

  [[nodiscard]] std::string ToJson() override;

private:
  inline void Visit(ReflectorsControllerCommandVisitor& visitor) override { visitor.ProcessCommand(*this); }

  std::string _bus_name;
};

void to_json(nlohmann::json& j, const StopMovementCommand& command);

void from_json(const nlohmann::json& j, StopMovementCommand& command);

}  // namespace voltiris::controller