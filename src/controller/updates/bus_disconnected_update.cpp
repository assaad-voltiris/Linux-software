#include "bus_disconnected_update.hpp"

namespace voltiris::controller {

std::string BusDisconnectedUpdate::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const BusDisconnectedUpdate& update) { j = nlohmann::json{{"id", "BusDisconnectedUpdate"}}; }

void from_json(const nlohmann::json& j, BusDisconnectedUpdate& update) { update = BusDisconnectedUpdate(); }

}  // namespace voltiris::controller