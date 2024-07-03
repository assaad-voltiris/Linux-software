#include "connect_command.hpp"

namespace voltiris::controller {

std::string ConnectCommand::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const ConnectCommand& command) { j = nlohmann::json{{"id", "ConnectCommand"}, {"bus_name", command.GetBusName()}}; }

void from_json(const nlohmann::json& j, ConnectCommand& command) { command = ConnectCommand(j.at("bus_name").get<std::string>()); }

}  // namespace voltiris::controller