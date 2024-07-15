#include "load_configuration_command.hpp"

namespace voltiris::controller {

std::string LoadConfigurationCommand::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const LoadConfigurationCommand& command) {
  j = nlohmann::json{{"id", "LoadConfiguration"}, {"p", command.GetFilePath()}, {"c", command.GetFileContent()}};
}

void from_json(const nlohmann::json& j, LoadConfigurationCommand& command) {
  command = LoadConfigurationCommand(j.at("p").get<std::string>(), j.at("c").get<std::string>());
}

}  // namespace voltiris::controller