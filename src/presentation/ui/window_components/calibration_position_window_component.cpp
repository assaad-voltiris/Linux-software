//
// Created by sergi on 5/22/2024.
//

#include "calibration_position_window_component.hpp"

#include <imgui.h>

#include <controller/commands/values_update_command.hpp>

namespace voltiris::presentation::ui {

void CalibrationPositionWindowComponent::Render(double scale) {
  // General group for CalibrationPosition
  ImGui::BeginGroup();

  ImGui::BeginChild("Calibration Position", {}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle);
  ImGui::SeparatorText("Calibration Position");

  ImGui::InputDouble("AZIMUTH Motor position (mm)", &_azimuth_motor_position);
  ImGui::InputDouble("AZIMUTH Angle (°)", &_azimuth_angle);

  ImGui::InputDouble("ELEVATION Motor position (mm)", &_elevation_motor_position);
  ImGui::InputDouble("ELEVATION Angle (°)", &_elevation_angle);

  ImGui::Text("MOTOR POSITION: RefAZ=%d/RefEL=%d", 400, 200);

  ImGui::EndChild();
  ImGui::EndGroup();
}
}  // namespace voltiris::presentation::ui
