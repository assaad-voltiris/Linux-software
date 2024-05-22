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
    : WindowComponent(commands_handler) {
  std::vector<controller::ValuesUpdateCommand::ValueUpdate> initial_updates;
  initial_updates.emplace_back(controller::ValuesUpdateCommand::ValueId::kCycleFrequency, std::stod(kACSCycleFrequency[_cycle_frequency_current_index]));
  initial_updates.emplace_back(controller::ValuesUpdateCommand::ValueId::kStartingHraEnabled, _starting_hra_enabled);
  initial_updates.emplace_back(controller::ValuesUpdateCommand::ValueId::kStartingHra, _starting_hra);
  initial_updates.emplace_back(controller::ValuesUpdateCommand::ValueId::kAccelerationFactor,
                               std::stod(kACSAccelerationFactor[_acceleration_factor_current_index]));
  SendCommand(std::make_unique<controller::ValuesUpdateCommand>(initial_updates));
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
    SendCommand(std::make_unique<controller::ValuesUpdateCommand>(
        std::vector<controller::ValuesUpdateCommand::ValueUpdate>{{controller::ValuesUpdateCommand::ValueId::kStartingHraEnabled, _starting_hra_enabled}}));
  }
  ImGui::BeginDisabled(!_starting_hra_enabled);
  if (ImGui::InputDouble("Starting Hra", &_starting_hra)) {
    SendCommand(std::make_unique<controller::ValuesUpdateCommand>(
        std::vector<controller::ValuesUpdateCommand::ValueUpdate>{{controller::ValuesUpdateCommand::ValueId::kStartingHra, _starting_hra}}));
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
