//
// Created by sergi on 5/22/2024.
//

#include "accelerated_tracking_settings_window_component.hpp"

#include <imgui.h>

#include <controller/commands/automatic_move_command.hpp>
#include <controller/commands/stop_movement_command.hpp>
#include <controller/commands/values_update_command.hpp>
#include <presentation/ui/utils/size.hpp>

namespace voltiris::presentation::ui {

namespace {

const char *kACSCycleFrequency[] = {"1", "2", "7", "OFF"};

}  // namespace

AcceleratedTrackingSettingsWindowComponent::AcceleratedTrackingSettingsWindowComponent(controller::ControllerCommandsHandler &commands_handler)
    : WindowComponent(commands_handler) {}

void AcceleratedTrackingSettingsWindowComponent::OnCycleFrequency(double value) {
  for (std::int32_t i = 0; i < IM_ARRAYSIZE(kACSCycleFrequency); i++) {
    if (value == std::stod(kACSCycleFrequency[i])) {
      _cycle_frequency_current_index = i;
      return;
    }
  }
  _cycle_frequency_current_index = IM_ARRAYSIZE(kACSCycleFrequency) - 1;
}

void AcceleratedTrackingSettingsWindowComponent::OnStartingHraEnabled(bool value) { _starting_hra_enabled = value; }

void AcceleratedTrackingSettingsWindowComponent::OnStartingHra(double value) { _starting_hra = value; }

void AcceleratedTrackingSettingsWindowComponent::OnAccelerationFactor(std::int32_t value) { _acceleration_factor = value; }

void AcceleratedTrackingSettingsWindowComponent::Render(double scale) {
  ImGui::BeginDisabled(GetControllerStatus() == controller::ControllerStatus::kTracking);

  // General group for AcceleratedTrackingSettings
  ImGui::BeginGroup();

  // ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260)
  ImGui::BeginChild("Accelerated Tracking Settings", {}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle);
  ImGui::SeparatorText("Accelerated Tracking Settings");

  // Left column
  ImGui::BeginGroup();
  if (ImGui::Button("MOVE", utils::GetRealSize({60, 40}, static_cast<float>(scale)))) {
    SendCommand(
        std::make_unique<controller::AutomaticMoveCommand>(_checkbox_400_0, _checkbox_to_theoretical_position, _checkbox_90_deg, _checkbox_initial_position));
  }

  if (ImGui::Button("STOP", utils::GetRealSize({60, 40}, static_cast<float>(scale)))) { SendCommand(std::make_unique<controller::StopMovementCommand>()); }
  ImGui::EndGroup();

  ImGui::SameLine();

  // Center column
  ImGui::SameLine();
  ImGui::BeginGroup();

  ImGui::Checkbox("400-0", &_checkbox_400_0);

  if (ImGui::Checkbox("to th. pos.", &_checkbox_to_theoretical_position) && _checkbox_to_theoretical_position) {
    _checkbox_90_deg = false;
    _checkbox_initial_position = false;
  }

  if (ImGui::Checkbox("Perp. to Sun", &_checkbox_90_deg) && _checkbox_90_deg) {
    _checkbox_to_theoretical_position = false;
    _checkbox_initial_position = false;
  }

  if (ImGui::Checkbox("to init pos.", &_checkbox_initial_position) && _checkbox_initial_position) {
    _checkbox_to_theoretical_position = false;
    _checkbox_90_deg = false;
  }

  ImGui::EndGroup();

  // Bottom line
  ImGui::BeginGroup();
  if (ImGui::BeginCombo("Cycles to auto calibration", kACSCycleFrequency[_cycle_frequency_current_index], ImGuiComboFlags_WidthFitPreview)) {
    for (std::int32_t i = 0; i < IM_ARRAYSIZE(kACSCycleFrequency); i++) {
      const bool is_selected = (_cycle_frequency_current_index == i);
      if (ImGui::Selectable(kACSCycleFrequency[i], is_selected)) { _cycle_frequency_current_index = i; }
      if (is_selected) { ImGui::SetItemDefaultFocus(); }
    }
    ImGui::EndCombo();
  }

  if (ImGui::Checkbox("##Enable Starting Hra", &_starting_hra_enabled)) {
    controller::ValuesUpdateCommand cmd;
    cmd.SetStartingHra(_starting_hra_enabled ? _starting_hra : -1);
    SendCommand(std::make_unique<controller::ValuesUpdateCommand>(cmd));
  }

  ImGui::SameLine();

  ImGui::BeginDisabled(!_starting_hra_enabled);
  if (ImGui::InputDouble("Starting Hra", &_starting_hra)) {
    controller::ValuesUpdateCommand cmd;
    cmd.SetStartingHra(_starting_hra_enabled ? _starting_hra : -1);
    SendCommand(std::make_unique<controller::ValuesUpdateCommand>(cmd));
  }
  ImGui::EndDisabled();

  if (ImGui::InputInt("Acceleration Factor", &_acceleration_factor)) {
    controller::ValuesUpdateCommand cmd;
    cmd.SetAccelerationFactor(_acceleration_factor);
    SendCommand(std::make_unique<controller::ValuesUpdateCommand>(cmd));
  }

  ImGui::EndGroup();

  ImGui::EndChild();
  ImGui::EndGroup();

  ImGui::EndDisabled();
}

}  // namespace voltiris::presentation::ui
