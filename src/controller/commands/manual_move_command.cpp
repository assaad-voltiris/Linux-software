#include "manual_move_command.hpp"

namespace voltiris::controller {

std::string ManualMoveCommand::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const ManualMoveCommand& command) {
  j = nlohmann::json{{"id", "ManualMove"}, {"d", static_cast<std::uint32_t>(command.GetDirection())}, {"s", command.GetStep()}, {"r", command.GetReflectors()}};
}

void from_json(const nlohmann::json& j, ManualMoveCommand& command) {
  command =
      ManualMoveCommand(static_cast<ManualMoveCommand::Direction>(j["d"].get<std::uint32_t>()), j["s"].get<double>(), j["r"].get<std::vector<std::size_t>>());
}

}  // namespace voltiris::controller