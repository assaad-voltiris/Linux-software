#include "values_update_command.hpp"

namespace voltiris::controller {

std::string ValuesUpdateCommand::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const ValuesUpdateCommand& command) {
  nlohmann::json json;
  json["id"] = "ValuesUpdate";

  if (command.GetStartingHra().has_value()) { json["h"] = command.GetStartingHra().value(); }
  if (command.GetAccelerationFactor().has_value()) { json["a"] = command.GetAccelerationFactor().value(); }

  j = json;
}

void from_json(const nlohmann::json& j, ValuesUpdateCommand& command) {
  if (j.contains("h")) { command.SetStartingHra(j["h"].get<double>()); }
  if (j.contains("a")) { command.SetAccelerationFactor(j["a"].get<std::int32_t>()); }
}

}  // namespace voltiris::controller