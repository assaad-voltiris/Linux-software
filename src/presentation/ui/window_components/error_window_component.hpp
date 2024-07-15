//
// Created by sergi on 5/22/2024.
//

#pragma once

#include <string>

#include <imgui.h>

#include <controller/controller_commands_handler.hpp>

#include <presentation/ui/resources.hpp>
#include <presentation/ui/window_components/window_component.hpp>

namespace voltiris::presentation::ui {

class ErrorWindowComponent : public WindowComponent {
public:
  explicit ErrorWindowComponent(controller::ControllerCommandsHandler& commands_handler) : WindowComponent(commands_handler) {}

  void Render(double scale) override;

  void OnControllerStatusUpdate(controller::ControllerStatus status) override { _controller_status = status; }

  // RENDERING LOGIC

  // DATA
  void OnError(const std::string& error) {
    _current_error = error;
    _show_popup = true;
  }

private:
  controller::ControllerStatus _controller_status = controller::ControllerStatus::kIdle;

  // RENDERING LOGIC
  bool _show_popup = false;

  // DATA
  std::string _current_error;
};

}  // namespace voltiris::presentation::ui
