#include "current_configuration_update.hpp"

namespace voltiris::controller {

std::string CurrentConfigurationUpdate::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const CurrentConfigurationUpdate& update) {
  j = nlohmann::json{{"id", "CurrentConfigurationUpdate"},
                     {"latitude", update.GetLatitude()},
                     {"longitude", update.GetLongitude()},
                     {"cycle_frequency", update.GetCycleFrequency()},
                     {"starting_hra_enabled", update.GetStartingHraEnabled()},
                     {"starting_hra", update.GetStartingHra()},
                     {"acceleration_factor", update.GetAccelerationFactor()}};
}

void from_json(const nlohmann::json& j, CurrentConfigurationUpdate& update) {
  update = CurrentConfigurationUpdate(j.at("latitude").get<double>(), j.at("latitude").get<double>(), j.at("cycle_frequency").get<double>(),
                                      j.at("starting_hra_enabled").get<bool>(), j.at("starting_hra").get<double>(), j.at("acceleration_factor").get<double>());
}

}  // namespace voltiris::controller