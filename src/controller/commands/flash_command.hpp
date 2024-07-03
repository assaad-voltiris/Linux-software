#pragma once

#include <string>

#include <nlohmann/json.hpp>

#include <controller/commands/reflectors_controller_command.hpp>
#include <controller/commands/reflectors_controller_command_visitor.hpp>

namespace voltiris::controller {

class FlashCommand : public ReflectorsControllerCommand {
public:
  inline explicit FlashCommand() = default;
  ~FlashCommand() override = default;

  [[nodiscard]] std::string ToJson() override;

private:
  inline void Visit(ReflectorsControllerCommandVisitor& visitor) override { visitor.ProcessCommand(*this); }
};

void to_json(nlohmann::json& j, const FlashCommand& command);

void from_json(const nlohmann::json& j, FlashCommand& command);

}  // namespace voltiris::controller