#include "load_configuration_command.hpp"

namespace voltiris::controller {

std::string LoadConfigurationCommand::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const LoadConfigurationCommand& command) {
  j = nlohmann::json{{"id", "LoadConfigurationCommand"}, {"file_path", command.GetFilePath()}, {"file_content", command.GetFileContent()}};
}

void from_json(const nlohmann::json& j, LoadConfigurationCommand& command) {
  command = LoadConfigurationCommand(j.at("file_path").get<std::string>(), j.at("file_content").get<std::string>());
}

}  // namespace voltiris::controller