#include "stop_movement_command.hpp"

namespace voltiris::controller {

std::string StopMovementCommand::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const StopMovementCommand& command) { j = nlohmann::json{{"id", "StopMovement"}}; }

void from_json(const nlohmann::json& j, StopMovementCommand& command) { command = StopMovementCommand(); }

}  // namespace voltiris::controller