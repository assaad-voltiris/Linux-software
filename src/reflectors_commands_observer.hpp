#pragma once

#include <memory>

#include <commands/reflectors_controller_command.hpp>

namespace voltiris {

class ReflectorsCommandsObserver {
public:
  virtual void OnCommand(std::unique_ptr<ReflectorsControllerCommand> command) = 0;
};

}