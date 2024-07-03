#include "ast_update.hpp"
#include "bus_connected_update.hpp"

namespace voltiris::controller {

std::string ASTUpdate::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const ASTUpdate& update) {
  j = nlohmann::json{{"id", "ASTUpdate"},
                     {"latitude", update.GetLatitude()},
                     {"longitude", update.GetLongitude()},
                     {"local_hours", update.GetLocalHours()},
                     {"local_minutes", update.GetLocalMinutes()},
                     {"system_hours", update.GetSystemHours()},
                     {"system_minutes", update.GetSystemMinutes()},
                     {"sun_azimuth", update.GetSunAzimuth()},
                     {"sun_elevation", update.GetSunElevation()},
                     {"sun_ref_azimuth", update.GetSunRefAzimuth()},
                     {"sun_ref_elevation", update.GetSunRefElevation()},
                     {"hra", update.GetHra()},
                     {"hrar", update.GetHrar()}};
}

void from_json(const nlohmann::json& j, ASTUpdate& update) {
  update = ASTUpdate(j.at("latitude").get<double>(), j.at("longitude").get<double>(), j.at("local_hours").get<std::int32_t>(),
                     j.at("local_minutes").get<std::int32_t>(), j.at("system_hours").get<std::int32_t>(), j.at("system_minutes").get<std::int32_t>(),
                     j.at("sun_azimuth").get<double>(), j.at("sun_elevation").get<double>(), j.at("sun_ref_azimuth").get<double>(),
                     j.at("sun_ref_elevation").get<double>(), j.at("hrar").get<double>(), j.at("hra").get<double>());
}

}  // namespace voltiris::controller