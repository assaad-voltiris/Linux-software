#include "set_position_command.hpp"

namespace voltiris::controller {

std::string SetPositionCommand::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const SetPositionCommand& command) {
  j = nlohmann::json{
      {"id", "SetPositionCommand"}, {"reflector_index", command.GetReflectorIndex()}, {"azimuth", command.GetAzimuth()}, {"elevation", command.GetElevation()}};
}

void from_json(const nlohmann::json& j, SetPositionCommand& command) {
  command = SetPositionCommand(j.at("reflector_index").get<std::size_t>(), j.at("azimuth").get<double>(), j.at("elevation").get<double>());
}

}  // namespace voltiris::controller