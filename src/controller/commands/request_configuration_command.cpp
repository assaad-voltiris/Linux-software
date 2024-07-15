#include "request_configuration_command.hpp"

namespace voltiris::controller {

std::string RequestConfigurationCommand::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const RequestConfigurationCommand& command) { j = nlohmann::json{{"id", "RequestConfiguration"}}; }

void from_json(const nlohmann::json& j, RequestConfigurationCommand& command) { command = RequestConfigurationCommand(); }

}  // namespace voltiris::controller