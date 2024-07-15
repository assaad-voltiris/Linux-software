#include "bus_list_update.hpp"

namespace voltiris::controller {

std::string BusListUpdate::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const BusListUpdate& update) {
  nlohmann::json json;
  json["id"] = "BusList";
  json["b"] = {};
  for (const auto& bus : update.GetAvailableBuss()) { json["b"].push_back(bus); }

  j = json;
}

void from_json(const nlohmann::json& j, BusListUpdate& update) {
  std::vector<std::string> buss;
  for (auto& json_bus : j["b"]) { buss.emplace_back(json_bus); }
  update = BusListUpdate(buss);
}

}  // namespace voltiris::controller