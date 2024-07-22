#include "start_tracking_command.hpp"

namespace voltiris::controller {

std::string StartTrackingCommand::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const StartTrackingCommand& command) { j = nlohmann::json{{"id", "StartTracking"}}; }

void from_json(const nlohmann::json& j, StartTrackingCommand& command) { command = StartTrackingCommand(); }

}  // namespace voltiris::controller