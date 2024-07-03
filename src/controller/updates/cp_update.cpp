#include "cp_update.hpp"

namespace voltiris::controller {

std::string CPUpdate::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const CPUpdate& update) { j = nlohmann::json{{"id", "CPUpdate"}}; }

void from_json(const nlohmann::json& j, CPUpdate& update) { update = CPUpdate(); }

}  // namespace voltiris::controller