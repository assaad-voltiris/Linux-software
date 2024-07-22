//
// Created by sergi on 5/22/2024.
//

#include "controller_status_window_component.hpp"

#include <map>

#include <imgui.h>

namespace voltiris::presentation::ui {

std::string ToString(controller::ControllerStatus status) {
  static const std::map<controller::ControllerStatus, std::string> kStrings{
      {controller::ControllerStatus::kOperating, "Normal operating"},
      {controller::ControllerStatus::kMoving, "Moving reflectors"},
      {controller::ControllerStatus::kTracking, "Automatic tracking"},
      {controller::ControllerStatus::kCommunicating, "Communicating with reflectors"},
      {controller::ControllerStatus::kTrackingWaiting, "Automatic tracking (waiting for morning time)"},
      {controller::ControllerStatus::kTrackingReturningNight, "Automatic tracking (returning to morning position)"}};

  auto it = kStrings.find(status);
  if (it != kStrings.end()) { return it->second; }
  return "";
}

void ControllerStatusWindowComponent::Render(double scale) {
  // General group for CalibrationPosition
  ImGui::BeginGroup();

  ImGui::BeginChild("Status", {}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle);
  ImGui::Text("Current status: %s", _controller_status_str.c_str());

  ImGui::EndChild();
  ImGui::EndGroup();
}

void ControllerStatusWindowComponent::OnControllerStatusUpdate(controller::ControllerStatus status) {
  _controller_status = status;
  _controller_status_str = ToString(_controller_status);
}

}  // namespace voltiris::presentation::ui
