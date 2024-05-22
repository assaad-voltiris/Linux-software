#pragma once

#include <controller/commands/reflectors_controller_command.hpp>
#include <memory>

namespace voltiris::controller {

class ControllerCommandsHandler {
public:
  virtual void OnCommand(std::unique_ptr<ReflectorsControllerCommand> command) = 0;
};

}  // namespace voltiris::controller