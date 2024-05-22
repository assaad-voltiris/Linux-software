//
// Created by sergi on 5/22/2024.
//

#include "manual_reflectors_alignment_window_component.hpp"

#include <imgui.h>

#include <controller/commands/values_update_command.hpp>

namespace voltiris::presentation::ui {

void ManualReflectorsAlignmentWindowComponent::Render(double scale) {  // General group for AcceleratedTrackingSettings
  ImGui::BeginGroup();

  ImGui::BeginChild("Manual Reflectors Alignment", {}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle);
  ImGui::SeparatorText("Manual Reflectors Alignment");

  if (ImGui::Button("UP")) {}
  ImGui::SameLine();
  if (ImGui::Button("DOWN")) {}
  ImGui::SameLine();
  if (ImGui::Button("EAST")) {}
  ImGui::SameLine();
  if (ImGui::Button("WEST")) {}

  ImGui::BeginGroup();
  ImGui::InputDouble("Step size (°)", &_step_size);
  ImGui::InputText("Reflectors To Move", _reflectors_to_move, IM_ARRAYSIZE(_reflectors_to_move));
  ImGui::EndGroup();

  ImGui::EndChild();
  ImGui::EndGroup();
}

}  // namespace voltiris::presentation::ui
