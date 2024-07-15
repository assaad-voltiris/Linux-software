//
// Created by sergi on 5/22/2024.
//
#pragma once

#include <string>
#include <vector>

#include <controller/controller_commands_handler.hpp>

#include <presentation/ui/resources.hpp>
#include <presentation/ui/window_components/window_component.hpp>

namespace voltiris::presentation::ui {

class CalibrationPositionWindowComponent : public WindowComponent {
public:
  explicit CalibrationPositionWindowComponent(controller::ControllerCommandsHandler& commands_handler) : WindowComponent(commands_handler) {}

  void Render(double scale) override;

  void OnControllerStatusUpdate(controller::ControllerStatus status) override { _controller_status = status; }

  // RENDERING LOGIC

  // DATA

private:
  controller::ControllerStatus _controller_status = controller::ControllerStatus::kIdle;

  // RENDERING LOGIC
  double _azimuth_motor_position = 0;
  double _azimuth_angle = 0;
  double _elevation_motor_position = 0;
  double _elevation_angle = 0;

  // DATA
};

}  // namespace voltiris::presentation::ui

