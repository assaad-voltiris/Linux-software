//
// Created by sergi on 5/22/2024.
//

#pragma once

#include <controller/controller_commands_handler.hpp>
#include <controller/controller_status.hpp>

namespace voltiris::presentation::ui {

class WindowComponent {
public:
  explicit WindowComponent(controller::ControllerCommandsHandler& commands_handler) : _commands_handler(commands_handler) {}

  // virtual bool Init() = 0;

  virtual void Render(double scale) = 0;
  virtual void OnControllerStatusUpdate(controller::ControllerStatus status) { _controller_status = status; }

protected:
  void SendCommand(std::unique_ptr<controller::ReflectorsControllerCommand> command) { _commands_handler.OnCommand(std::move(command)); }

  [[nodiscard]] controller::ControllerStatus GetControllerStatus() const { return _controller_status; }

private:
  controller::ControllerCommandsHandler& _commands_handler;
  controller::ControllerStatus _controller_status = controller::ControllerStatus::kOperating;
};

}  // namespace voltiris::presentation::ui
