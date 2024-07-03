//
// Created by sergi on 5/22/2024.
//

#pragma once

#include <controller/controller_commands_handler.hpp>

namespace voltiris::presentation::ui {

class WindowComponent {
public:
  explicit WindowComponent(controller::ControllerCommandsHandler& commands_handler) : _commands_handler(commands_handler) {}

  void SendCommand(std::unique_ptr<controller::ReflectorsControllerCommand> command) { _commands_handler.OnCommand(std::move(command)); }

  //virtual bool Init() = 0;

  virtual void Render(double scale) = 0;

private:
  controller::ControllerCommandsHandler& _commands_handler;
};

}  // namespace voltiris::presentation::ui
