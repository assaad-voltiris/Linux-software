#include "move_command.hpp"

namespace voltiris::controller {

std::string MoveCommand::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const MoveCommand& command) {
  j = nlohmann::json{
      {"id", "MoveCommand"}, {"d", static_cast<std::uint32_t>(command.GetDirection())}, {"s", command.GetStep()}, {"r", command.GetReflectors()}};
}

void from_json(const nlohmann::json& j, MoveCommand& command) {
  command = MoveCommand(static_cast<MoveCommand::Direction>(j["d"].get<std::uint32_t>()), j["s"].get<double>(), j["r"].get<std::vector<std::size_t>>());
}

}  // namespace voltiris::controller