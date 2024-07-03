#pragma once

#include <string>

#include <nlohmann/json.hpp>

#include <controller/commands/reflectors_controller_command.hpp>
#include <controller/commands/reflectors_controller_command_visitor.hpp>

namespace voltiris::controller {

class RebootCommand : public ReflectorsControllerCommand {
public:
  inline explicit RebootCommand() = default;
  ~RebootCommand() override = default;

  [[nodiscard]] std::string ToJson() override;

private:
  inline void Visit(ReflectorsControllerCommandVisitor& visitor) override { visitor.ProcessCommand(*this); }
};

void to_json(nlohmann::json& j, const RebootCommand& command);

void from_json(const nlohmann::json& j, RebootCommand& command);

}  // namespace voltiris::controller