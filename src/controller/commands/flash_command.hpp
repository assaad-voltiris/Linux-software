#pragma once

#include <string>

#include <controller/commands/reflectors_controller_command.hpp>
#include <controller/commands/reflectors_controller_command_visitor.hpp>

namespace voltiris::controller {

class FlashCommand : public ReflectorsControllerCommand {
public:
  inline explicit FlashCommand() = default;
  ~FlashCommand() override = default;

private:
  inline void Visit(ReflectorsControllerCommandVisitor& visitor) override { visitor.ProcessCommand(*this); }
};

}  // namespace voltiris::controller