#include "disconnect_command.hpp"

namespace voltiris::controller {

std::string DisconnectCommand::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const DisconnectCommand& command) { j = nlohmann::json{{"id", "DisconnectCommand"}}; }

void from_json(const nlohmann::json& j, DisconnectCommand& command) { command = DisconnectCommand(); }

}  // namespace voltiris::controller