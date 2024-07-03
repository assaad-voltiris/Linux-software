#include "go_command.hpp"

namespace voltiris::controller {

std::string GoCommand::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const GoCommand& command) {
  j = nlohmann::json{
      {"id", "GoCommand"}, {"reflector_index", command.GetReflectorIndex()}, {"azimuth", command.GetAzimuth()}, {"elevation", command.GetElevation()}};
}

void from_json(const nlohmann::json& j, GoCommand& command) {
  command = GoCommand(j.at("reflector_index").get<std::size_t>(), j.at("azimuth").get<double>(), j.at("elevation").get<double>());
}

}  // namespace voltiris::controller