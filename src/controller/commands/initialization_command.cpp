#include "initialization_command.hpp"

namespace voltiris::controller {

std::string InitializationCommand::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const InitializationCommand& command) { j = nlohmann::json{{"id", "Initialization"}}; }

void from_json(const nlohmann::json& j, InitializationCommand& command) { command = InitializationCommand(); }

}  // namespace voltiris::controller