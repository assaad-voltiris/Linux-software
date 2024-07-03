#include "reboot_command.hpp"

namespace voltiris::controller {

std::string RebootCommand::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const RebootCommand& command) { j = nlohmann::json{{"id", "RebootCommand"}}; }

void from_json(const nlohmann::json& j, RebootCommand& command) { command = RebootCommand(); }

}  // namespace voltiris::controller