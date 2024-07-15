#include "error_update.hpp"

namespace voltiris::controller {

std::string ErrorUpdate::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const ErrorUpdate& update) { j = nlohmann::json{{"id", "Error"}, {"e", update.GetError()}}; }

void from_json(const nlohmann::json& j, ErrorUpdate& update) { update = ErrorUpdate(j.at("e").get<std::string>()); }

}  // namespace voltiris::controller