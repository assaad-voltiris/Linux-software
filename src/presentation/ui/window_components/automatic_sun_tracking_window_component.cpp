//
// Created by sergi on 5/22/2024.
//

#include "automatic_sun_tracking_window_component.hpp"

#include <imgui.h>

#include <controller/commands/values_update_command.hpp>

namespace voltiris::presentation::ui {

AutomaticSunTrackingWindowComponent::AutomaticSunTrackingWindowComponent(controller::ControllerCommandsHandler& commands_handler)
    : WindowComponent(commands_handler) {
  std::vector<controller::ValuesUpdateCommand::ValueUpdate> initial_updates;
  initial_updates.emplace_back(controller::ValuesUpdateCommand::ValueId::kLatitude, _latitude);
  initial_updates.emplace_back(controller::ValuesUpdateCommand::ValueId::kLongitude, _longitude);
  SendCommand(std::make_unique<controller::ValuesUpdateCommand>(initial_updates));
}

void AutomaticSunTrackingWindowComponent::Render(double scale) {
  // General group for AutomaticSunTracking
  ImGui::BeginGroup();

  ImGui::BeginChild("Automatic Sun Tracking", {}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle);
  ImGui::SeparatorText("Automatic Sun Tracking");

  ImGui::InputDouble("Latitude (°)", &_latitude);
  ImGui::InputDouble("Longitude (°)", &_longitude);

  if (ImGui::Button("START TRACKING")) {}
  ImGui::SameLine();
  if (ImGui::Button("STOP TRACKING")) {}

  if (ImGui::Button("Count Cycles")) {}
  ImGui::SameLine();
  ImGui::BeginDisabled();
  ImGui::InputInt("##count_cycles", &_count_cycles);
  ImGui::EndDisabled();

  ImGui::Text("LT %02dh%02dm, ST %02dh%02dm", _local_time_h, _local_time_m, _system_time_h, _system_time_m);
  ImGui::Text("Sun AZ %3.3f° / EL %3.3f°", _sun_azimuth, _sun_elevation);
  ImGui::Text("Ref AZ %3.3f° / EL %3.3f°", _sun_ref_azimuth, _sun_ref_elevation);
  ImGui::Text("hra %3.3f° / hrar %3.3f°", _hra, _hrar);

  ImGui::EndChild();
  ImGui::EndGroup();
}

}  // namespace voltiris::presentation::ui
