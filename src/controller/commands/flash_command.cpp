#include "flash_command.hpp"

namespace voltiris::controller {

std::string FlashCommand::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const FlashCommand& command) { j = nlohmann::json{{"id", "Flash"}}; }

void from_json(const nlohmann::json& j, FlashCommand& command) { command = FlashCommand(); }

}  // namespace voltiris::controller