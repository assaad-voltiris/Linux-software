#include "values_update_command.hpp"

namespace voltiris::controller {

std::string ValuesUpdateCommand::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const ValuesUpdateCommand& command) {
  nlohmann::json json;
  json["id"] = "ValuesUpdateCommand";
  json["updates"] = {};
  for (const auto& update : command.GetUpdates()) {
    json["updates"].push_back({{"value_id", static_cast<std::uint32_t>(update.value_id)}, {"value", update.value_uint}});
  }

  j = json;
}

void from_json(const nlohmann::json& j, ValuesUpdateCommand& command) {
  std::vector<ValuesUpdateCommand::ValueUpdate> updates;
  for (auto& json_update : j["updates"]) {
    updates.emplace_back(static_cast<ValuesUpdateCommand::ValueId>(json_update["value_id"]), json_update["value"].get<std::size_t>());
  }
  command = ValuesUpdateCommand(updates);
}

}  // namespace voltiris::controller