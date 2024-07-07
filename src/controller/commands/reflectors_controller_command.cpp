#include "reflectors_controller_command.hpp"

#include <nlohmann/json.hpp>

#include <controller/commands/connect_command.hpp>
#include <controller/commands/disconnect_command.hpp>
#include <controller/commands/flash_command.hpp>
#include <controller/commands/go_command.hpp>
#include <controller/commands/initialization_command.hpp>
#include <controller/commands/load_configuration_command.hpp>
#include <controller/commands/move_command.hpp>
#include <controller/commands/read_command.hpp>
#include <controller/commands/reboot_command.hpp>
#include <controller/commands/request_configuration_command.hpp>
#include <controller/commands/set_position_command.hpp>
#include <controller/commands/values_update_command.hpp>

namespace voltiris::controller {

std::unique_ptr<ReflectorsControllerCommand> ReflectorsControllerCommand::FromJson(const std::string& command_json) {
  nlohmann::json json = nlohmann::json::parse(command_json);

  if (json["id"] == "LoadConfigurationCommand") {
    return std::make_unique<LoadConfigurationCommand>(json.template get<LoadConfigurationCommand>());
  } else if (json["id"] == "ValuesUpdateCommand") {
    return std::make_unique<ValuesUpdateCommand>(json.template get<ValuesUpdateCommand>());
  } else if (json["id"] == "ConnectCommand") {
    return std::make_unique<ConnectCommand>(json.template get<ConnectCommand>());
  } else if (json["id"] == "DisconnectCommand") {
    return std::make_unique<DisconnectCommand>(json.template get<DisconnectCommand>());
  } else if (json["id"] == "InitializationCommand") {
    return std::make_unique<InitializationCommand>(json.template get<InitializationCommand>());
  } else if (json["id"] == "ReadCommand") {
    return std::make_unique<ReadCommand>(json.template get<ReadCommand>());
  } else if (json["id"] == "FlashCommand") {
    return std::make_unique<FlashCommand>(json.template get<FlashCommand>());
  } else if (json["id"] == "RebootCommand") {
    return std::make_unique<RebootCommand>(json.template get<RebootCommand>());
  } else if (json["id"] == "SetPositionCommand") {
    return std::make_unique<SetPositionCommand>(json.template get<SetPositionCommand>());
  } else if (json["id"] == "GoCommand") {
    return std::make_unique<GoCommand>(json.template get<GoCommand>());
  } else if (json["id"] == "RequestConfigurationCommand") {
    return std::make_unique<RequestConfigurationCommand>(json.template get<RequestConfigurationCommand>());
  } else if (json["id"] == "MoveCommand") {
    return std::make_unique<MoveCommand>(json.template get<MoveCommand>());
  }
  return nullptr;
}

}  // namespace voltiris::controller