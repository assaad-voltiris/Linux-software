#include "stop_tracking_command.hpp"

namespace voltiris::controller {

std::string StopTrackingCommand::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const StopTrackingCommand& command) { j = nlohmann::json{{"id", "StopTracking"}}; }

void from_json(const nlohmann::json& j, StopTrackingCommand& command) { command = StopTrackingCommand(); }

}  // namespace voltiris::controller