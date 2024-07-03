#include "status_update.hpp"

namespace voltiris::controller {

std::string StatusUpdate::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const StatusUpdate& update) {
  j = nlohmann::json{{"id", "StatusUpdate"}, {"status", static_cast<std::uint32_t>(update.GetStatus())}};
}

void from_json(const nlohmann::json& j, StatusUpdate& update) { update = StatusUpdate(static_cast<ControllerStatus>(j.at("status").get<std::uint32_t>())); }

}  // namespace voltiris::controller