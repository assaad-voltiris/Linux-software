#include "reflectors_size_update.hpp"

namespace voltiris::controller {

std::string ReflectorsSizeUpdate::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const ReflectorsSizeUpdate& update) { j = nlohmann::json{{"id", "ReflectorsSizeUpdate"}, {"size", update.GetSize()}}; }

void from_json(const nlohmann::json& j, ReflectorsSizeUpdate& update) { update = ReflectorsSizeUpdate(j.at("size").get<std::size_t>()); }

}  // namespace voltiris::controller