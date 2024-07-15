//
// Created by sergi on 5/22/2024.
//

#pragma once

#include <string>

#include <controller/controller_commands_handler.hpp>

#include <presentation/ui/resources.hpp>
#include <presentation/ui/window_components/window_component.hpp>

namespace voltiris::presentation::ui {

class ControllerStatusWindowComponent : public WindowComponent {
public:
  explicit ControllerStatusWindowComponent(controller::ControllerCommandsHandler& commands_handler) : WindowComponent(commands_handler) {}

  void Render(double scale) override;

  void OnControllerStatusUpdate(controller::ControllerStatus status) override;

  // RENDERING LOGIC

  // DATA

private:
  controller::ControllerStatus _controller_status = controller::ControllerStatus::kIdle;
  std::string _controller_status_str;

  // RENDERING LOGIC

  // DATA
};

}  // namespace voltiris::presentation::ui
