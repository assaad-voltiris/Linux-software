#include "read_command.hpp"

namespace voltiris::controller {

std::string ReadCommand::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const ReadCommand& command) { j = nlohmann::json{{"id", "ReadCommand"}}; }

void from_json(const nlohmann::json& j, ReadCommand& command) { command = ReadCommand(); }

}  // namespace voltiris::controller