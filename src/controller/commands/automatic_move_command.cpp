#include "automatic_move_command.hpp"

namespace voltiris::controller {

std::string AutomaticMoveCommand::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const AutomaticMoveCommand& command) {
  j = nlohmann::json{{"id", "AutomaticMove"},
                     {"c", command.GetCalibrate()},
                     {"t", command.GetTheoreticalPosition()},
                     {"p", command.GetPerpToSun()},
                     {"i", command.GetInitPosition()}};
}

void from_json(const nlohmann::json& j, AutomaticMoveCommand& command) {
  command = AutomaticMoveCommand(j["c"].get<bool>(), j["t"].get<bool>(), j["p"].get<bool>(), j["i"].get<bool>());
}

}  // namespace voltiris::controller