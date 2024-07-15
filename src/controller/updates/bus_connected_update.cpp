#include "bus_connected_update.hpp"

namespace voltiris::controller {

std::string BusConnectedUpdate::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const BusConnectedUpdate& update) { j = nlohmann::json{{"id", "BusConnected"}}; }

void from_json(const nlohmann::json& j, BusConnectedUpdate& update) { update = BusConnectedUpdate(); }

}  // namespace voltiris::controller