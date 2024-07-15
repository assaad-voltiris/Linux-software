#include "reflectors_controller_command.hpp"

#include <nlohmann/json.hpp>

#include <controller/commands/connect_command.hpp>
#include <controller/commands/disconnect_command.hpp>
#include <controller/commands/flash_command.hpp>
#include <controller/commands/go_command.hpp>
#include <controller/commands/initialization_command.hpp>
#include <controller/commands/load_configuration_command.hpp>
#include <controller/commands/manual_move_command.hpp>
#include <controller/commands/read_command.hpp>
#include <controller/commands/reboot_command.hpp>
#include <controller/commands/request_configuration_command.hpp>
#include <controller/commands/set_position_command.hpp>
#include <controller/commands/values_update_command.hpp>

namespace voltiris::controller {

std::unique_ptr<ReflectorsControllerCommand> ReflectorsControllerCommand::FromJson(const std::string& command_json) {
  nlohmann::json json = nlohmann::json::parse(command_json);

  if (json["id"] == "LoadConfiguration") {
    return std::make_unique<LoadConfigurationCommand>(json.template get<LoadConfigurationCommand>());
  } else if (json["id"] == "ValuesUpdate") {
    return std::make_unique<ValuesUpdateCommand>(json.template get<ValuesUpdateCommand>());
  } else if (json["id"] == "Connect") {
    return std::make_unique<ConnectCommand>(json.template get<ConnectCommand>());
  } else if (json["id"] == "Disconnect") {
    return std::make_unique<DisconnectCommand>(json.template get<DisconnectCommand>());
  } else if (json["id"] == "Initialization") {
    return std::make_unique<InitializationCommand>(json.template get<InitializationCommand>());
  } else if (json["id"] == "Read") {
    return std::make_unique<ReadCommand>(json.template get<ReadCommand>());
  } else if (json["id"] == "Flash") {
    return std::make_unique<FlashCommand>(json.template get<FlashCommand>());
  } else if (json["id"] == "Reboot") {
    return std::make_unique<RebootCommand>(json.template get<RebootCommand>());
  } else if (json["id"] == "SetPosition") {
    return std::make_unique<SetPositionCommand>(json.template get<SetPositionCommand>());
  } else if (json["id"] == "Go") {
    return std::make_unique<GoCommand>(json.template get<GoCommand>());
  } else if (json["id"] == "RequestConfiguration") {
    return std::make_unique<RequestConfigurationCommand>(json.template get<RequestConfigurationCommand>());
  } else if (json["id"] == "ManualMove") {
    return std::make_unique<ManualMoveCommand>(json.template get<ManualMoveCommand>());
  }
  return nullptr;
}

}  // namespace voltiris::controller