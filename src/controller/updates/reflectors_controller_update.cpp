#include "reflectors_controller_update.hpp"

#include <nlohmann/json.hpp>

#include <controller/updates/ast_update.hpp>
#include <controller/updates/bus_connected_update.hpp>
#include <controller/updates/bus_disconnected_update.hpp>
#include <controller/updates/bus_list_update.hpp>
#include <controller/updates/cp_update.hpp>
#include <controller/updates/current_configuration_update.hpp>
#include <controller/updates/error_update.hpp>
#include <controller/updates/reflector_state_update.hpp>
#include <controller/updates/reflectors_size_update.hpp>
#include <controller/updates/status_update.hpp>

namespace voltiris::controller {

std::unique_ptr<ReflectorsControllerUpdate> ReflectorsControllerUpdate::FromJson(const std::string& update_json) {
  nlohmann::json json = nlohmann::json::parse(update_json);

  if (json["id"] == "CurrentConfigurationUpdate") {
    return std::make_unique<CurrentConfigurationUpdate>(json.template get<CurrentConfigurationUpdate>());
  } else if (json["id"] == "ASTUpdate") {
    return std::make_unique<ASTUpdate>(json.template get<ASTUpdate>());
  } else if (json["id"] == "BusConnectedUpdate") {
    return std::make_unique<BusConnectedUpdate>(json.template get<BusConnectedUpdate>());
  } else if (json["id"] == "BusDisconnectedUpdate") {
    return std::make_unique<BusDisconnectedUpdate>(json.template get<BusDisconnectedUpdate>());
  } else if (json["id"] == "BusListUpdate") {
    return std::make_unique<BusListUpdate>(json.template get<BusListUpdate>());
  } else if (json["id"] == "CPUpdate") {
    return std::make_unique<CPUpdate>(json.template get<CPUpdate>());
  } else if (json["id"] == "ReflectorStateUpdate") {
    return std::make_unique<ReflectorStateUpdate>(json.template get<ReflectorStateUpdate>());
  } else if (json["id"] == "ReflectorsSizeUpdate") {
    return std::make_unique<ReflectorsSizeUpdate>(json.template get<ReflectorsSizeUpdate>());
  } else if (json["id"] == "ErrorUpdate") {
    return std::make_unique<ErrorUpdate>(json.template get<ErrorUpdate>());
  } else if (json["id"] == "GoCommand") {
    return std::make_unique<StatusUpdate>(json.template get<StatusUpdate>());
  }
  return nullptr;
}

}  // namespace voltiris::controller
