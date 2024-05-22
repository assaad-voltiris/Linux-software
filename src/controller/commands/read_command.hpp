#pragma once

#include <string>

#include <controller/commands/reflectors_controller_command.hpp>
#include <controller/commands/reflectors_controller_command_visitor.hpp>

namespace voltiris::controller {

class ReadCommand : public ReflectorsControllerCommand {
public:
  inline explicit ReadCommand() {}
  ~ReadCommand() override = default;

private:
  inline void Visit(ReflectorsControllerCommandVisitor& visitor) override { visitor.ProcessCommand(*this); }
};

}  // namespace voltiris::controller