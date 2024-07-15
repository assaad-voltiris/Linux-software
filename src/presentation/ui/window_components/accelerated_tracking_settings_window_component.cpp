//
// Created by sergi on 5/22/2024.
//

#include "accelerated_tracking_settings_window_component.hpp"

#include <imgui.h>

#include <controller/commands/values_update_command.hpp>
#include <presentation/ui/utils/size.hpp>

namespace voltiris::presentation::ui {

namespace {

const char *kACSCycleFrequency[] = {"1", "2", "7", "OFF"};
const char *kACSAccelerationFactor[] = {"1", "4", "5", "6", "7", "8", "10", "12", "18", "24"};

}  // namespace

AcceleratedTrackingSettingsWindowComponent::AcceleratedTrackingSettingsWindowComponent(controller::ControllerCommandsHandler &commands_handler)
    : WindowComponent(commands_handler) {}

void AcceleratedTrackingSettingsWindowComponent::OnCycleFrequency(double value) {
  for (std::int32_t i = 0; i < IM_ARRAYSIZE(kACSCycleFrequency); i++) {
    if (value == std::stod(kACSCycleFrequency[i])) {
      _cycle_frequency_current_index = i;
      break;
    }
  }
  _cycle_frequency_current_index = IM_ARRAYSIZE(kACSCycleFrequency) - 1;
}

void AcceleratedTrackingSettingsWindowComponent::OnStartingHraEnabled(bool value) { _starting_hra_enabled = value; }

void AcceleratedTrackingSettingsWindowComponent::OnStartingHra(double value) { _starting_hra = value; }

void AcceleratedTrackingSettingsWindowComponent::OnAccelerationFactor(double value) {
  for (std::int32_t i = 0; i < IM_ARRAYSIZE(kACSAccelerationFactor); i++) {
    if (value == std::stod(kACSAccelerationFactor[i])) {
      _acceleration_factor_current_index = i;
      break;
    }
  }
  _acceleration_factor_current_index = IM_ARRAYSIZE(kACSAccelerationFactor) - 1;
}

void AcceleratedTrackingSettingsWindowComponent::Render(double scale) {
  // General group for AcceleratedTrackingSettings
  ImGui::BeginGroup();

  // ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260)
  ImGui::BeginChild("Accelerated Tracking Settings", {}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle);
  ImGui::SeparatorText("Accelerated Tracking Settings");

  // First line buttons
  ImGui::BeginGroup();
  if (ImGui::Button("Test")) {}

  ImGui::SameLine();
  if (ImGui::Button("# ##1")) {}

  ImGui::SameLine();
  if (ImGui::Button("# ##2")) {}
  ImGui::EndGroup();

  // Center group of items
  ImGui::BeginGroup();

  // Left column
  ImGui::BeginGroup();
  if (ImGui::Button("MOVE")) {}
  if (ImGui::BeginCombo("##Cycle frequency", kACSCycleFrequency[_cycle_frequency_current_index], ImGuiComboFlags_WidthFitPreview)) {
    for (std::int32_t i = 0; i < IM_ARRAYSIZE(kACSCycleFrequency); i++) {
      const bool is_selected = (_cycle_frequency_current_index == i);
      if (ImGui::Selectable(kACSCycleFrequency[i], is_selected)) { _cycle_frequency_current_index = i; }
      if (is_selected) { ImGui::SetItemDefaultFocus(); }
    }
    ImGui::EndCombo();
  }
  ImGui::EndGroup();

  // Center column
  ImGui::SameLine();
  ImGui::BeginGroup();

  ImGui::BeginDisabled(!_checkbox_400_0_enabled);
  ImGui::Checkbox("400-0", &_checkbox_400_0);
  ImGui::EndDisabled();

  ImGui::BeginDisabled(!_checkbox_to_theoretical_position_enabled);
  ImGui::Checkbox("to th. pos.", &_checkbox_to_theoretical_position);
  ImGui::EndDisabled();

  ImGui::BeginDisabled(!_checkbox_90_deg_enabled);
  ImGui::Checkbox("Perp. to Sun", &_checkbox_90_deg);
  ImGui::EndDisabled();

  ImGui::BeginDisabled(!_checkbox_initial_position_enabled);
  ImGui::Checkbox("to init pos.", &_checkbox_initial_position);
  ImGui::EndDisabled();

  ImGui::EndGroup();

  // Right column
  ImGui::SameLine();
  ImGui::BeginGroup();
  if (ImGui::Button("Move", utils::GetRealSize({80, 60}, static_cast<float>(scale)))) {}
  ImGui::EndGroup();
  ImGui::EndGroup();

  // Bottom line
  ImGui::BeginGroup();
  if (ImGui::Checkbox("Enable Starting Hra", &_starting_hra_enabled)) {
    controller::ValuesUpdateCommand cmd;
    cmd.SetStartingHra(_starting_hra_enabled ? _starting_hra : -1);
    SendCommand(std::make_unique<controller::ValuesUpdateCommand>(cmd));
  }
  ImGui::BeginDisabled(!_starting_hra_enabled);
  if (ImGui::InputDouble("Starting Hra", &_starting_hra)) {
    controller::ValuesUpdateCommand cmd;
    cmd.SetStartingHra(_starting_hra_enabled ? _starting_hra : -1);
    SendCommand(std::make_unique<controller::ValuesUpdateCommand>(cmd));
  }
  ImGui::EndDisabled();
  if (ImGui::BeginCombo("Acceleration Factor", kACSAccelerationFactor[_acceleration_factor_current_index], ImGuiComboFlags_WidthFitPreview)) {
    for (std::int32_t i = 0; i < IM_ARRAYSIZE(kACSAccelerationFactor); i++) {
      const bool is_selected = (_acceleration_factor_current_index == i);
      if (ImGui::Selectable(kACSAccelerationFactor[i], is_selected)) { _acceleration_factor_current_index = i; }
      if (is_selected) { ImGui::SetItemDefaultFocus(); }
    }
    ImGui::EndCombo();
  }
  ImGui::EndGroup();

  ImGui::EndChild();
  ImGui::EndGroup();
}

}  // namespace voltiris::presentation::ui
