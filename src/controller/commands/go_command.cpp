#include "go_command.hpp"

namespace voltiris::controller {

std::string GoCommand::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const GoCommand& command) {
  j = nlohmann::json{{"id", "Go"}, {"r", command.GetReflectorIndex()}, {"a", command.GetAzimuth()}, {"e", command.GetElevation()}};
}

void from_json(const nlohmann::json& j, GoCommand& command) {
  command = GoCommand(j.at("r").get<std::size_t>(), j.at("a").get<double>(), j.at("e").get<double>());
}

}  // namespace voltiris::controller