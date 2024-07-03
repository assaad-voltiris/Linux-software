#pragma once

#include <string>

#include <nlohmann/json.hpp>

#include <controller/commands/reflectors_controller_command.hpp>
#include <controller/commands/reflectors_controller_command_visitor.hpp>

namespace voltiris::controller {

class ReadCommand : public ReflectorsControllerCommand {
public:
  inline explicit ReadCommand() {}
  ~ReadCommand() override = default;

  [[nodiscard]] std::string ToJson() override;

private:
  inline void Visit(ReflectorsControllerCommandVisitor& visitor) override { visitor.ProcessCommand(*this); }
};

void to_json(nlohmann::json& j, const ReadCommand& command);

void from_json(const nlohmann::json& j, ReadCommand& command);

}  // namespace voltiris::controller