#include "set_position_command.hpp"

namespace voltiris::controller {

std::string SetPositionCommand::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const SetPositionCommand& command) {
  j = nlohmann::json{{"id", "SetPosition"}, {"r", command.GetReflectorIndex()}, {"a", command.GetAzimuth()}, {"e", command.GetElevation()}};
}

void from_json(const nlohmann::json& j, SetPositionCommand& command) {
  command = SetPositionCommand(j.at("r").get<std::size_t>(), j.at("a").get<double>(), j.at("e").get<double>());
}

}  // namespace voltiris::controller