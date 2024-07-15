#include "current_configuration_update.hpp"

namespace voltiris::controller {

std::string CurrentConfigurationUpdate::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const CurrentConfigurationUpdate& update) {
  j = nlohmann::json{
      {"id", "CurrentConfiguration"},           {"la", update.GetLatitude()},     {"lo", update.GetLongitude()},        {"c", update.GetCycleFrequency()},
      {"hrae", update.GetStartingHraEnabled()}, {"hra", update.GetStartingHra()}, {"a", update.GetAccelerationFactor()}};
}

void from_json(const nlohmann::json& j, CurrentConfigurationUpdate& update) {
  update = CurrentConfigurationUpdate(j.at("la").get<double>(), j.at("lo").get<double>(), j.at("c").get<double>(), j.at("hrae").get<bool>(),
                                      j.at("hra").get<double>(), j.at("a").get<double>());
}

}  // namespace voltiris::controller