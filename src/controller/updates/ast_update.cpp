#include "ast_update.hpp"
#include "bus_connected_update.hpp"

namespace voltiris::controller {

std::string ASTUpdate::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const ASTUpdate& update) {
  j = nlohmann::json{{"id", "AST"},
                     {"la", update.GetLatitude()},
                     {"lo", update.GetLongitude()},
                     {"lh", update.GetLocalHours()},
                     {"lm", update.GetLocalMinutes()},
                     {"sh", update.GetSystemHours()},
                     {"sm", update.GetSystemMinutes()},
                     {"a", update.GetSunAzimuth()},
                     {"e", update.GetSunElevation()},
                     {"ra", update.GetSunRefAzimuth()},
                     {"re", update.GetSunRefElevation()},
                     {"hra", update.GetHra()},
                     {"hrar", update.GetHrar()}};
}

void from_json(const nlohmann::json& j, ASTUpdate& update) {
  update = ASTUpdate(j.at("la").get<double>(), j.at("lo").get<double>(), j.at("lh").get<std::int32_t>(), j.at("lm").get<std::int32_t>(),
                     j.at("sh").get<std::int32_t>(), j.at("sm").get<std::int32_t>(), j.at("a").get<double>(), j.at("e").get<double>(), j.at("ra").get<double>(),
                     j.at("re").get<double>(), j.at("hra").get<double>(), j.at("hrar").get<double>());
}

}  // namespace voltiris::controller