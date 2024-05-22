//
// Created by sergi on 5/22/2024.
//
#include "configuration_window_component.hpp"

#include <imgui.h>

#include <portable-file-dialogs.h>

#include <controller/commands/connect_command.hpp>
#include <controller/commands/disconnect_command.hpp>
#include <controller/commands/load_configuration_command.hpp>

namespace voltiris::presentation::ui {

const char* ConfigurationWindowComponent::GetPopupName() { return "Reflectors Configuration"; }

void ConfigurationWindowComponent::Render(double scale) {
  // Temp variable to get cross on popup showed
  bool temp_show_cross = true;

  if (ImGui::BeginPopupModal("Reflectors Configuration", &temp_show_cross, ImGuiWindowFlags_AlwaysAutoResize)) {
    // Logging configuration
    if (ImGui::Button("Select Log Directory")) {
      auto dir = pfd::select_folder("Choose logs directory", pfd::path::home()).result();
      strcpy(_rc_logs_dir, dir.c_str());
    }

    ImGui::SameLine();

    ImGui::InputText("Logs Directory", _rc_logs_dir, IM_ARRAYSIZE(_rc_logs_dir), ImGuiInputTextFlags_ReadOnly);

    ImGui::SeparatorText("Reflectors Configuration");

    // Reflectors configuration
    if (ImGui::Button("Load reflectors configuration")) {
      auto file = pfd::open_file("Choose config file", pfd::path::home(), {"Text Files (.txt .text)", "*.txt *.text"}).result();
      if (!file.empty()) { SendCommand(std::make_unique<controller::LoadConfigurationCommand>(file[0])); }
    }

    ImGui::SameLine();

    ImGui::BeginDisabled(_is_connected);
    if (ImGui::BeginCombo("Bus port", _available_buss[_available_buss_current_index].c_str(), ImGuiComboFlags_WidthFitPreview)) {
      for (std::int32_t i = 0; i < _available_buss.size(); i++) {
        const bool is_selected = (_available_buss_current_index == i);
        if (ImGui::Selectable(_available_buss[i].c_str(), is_selected)) { _available_buss_current_index = i; }
        if (is_selected) { ImGui::SetItemDefaultFocus(); }
      }
      ImGui::EndCombo();
    }

    ImGui::SameLine();

    if (ImGui::Button("Connect")) { SendCommand(std::make_unique<controller::ConnectCommand>(_available_buss[_available_buss_current_index])); }
    ImGui::EndDisabled();

    ImGui::SameLine();

    ImGui::BeginDisabled(!_is_connected);
    if (ImGui::Button("Disconnect")) { SendCommand(std::make_unique<controller::DisconnectCommand>()); }
    ImGui::EndDisabled();

    ImGui::EndPopup();
  }
}

void ConfigurationWindowComponent::OnBusListUpdate(const std::vector<std::string>& available_buss) {
  _available_buss = available_buss;
  _available_buss_current_index = _available_buss.size() >= _available_buss_current_index ? 0 : _available_buss_current_index;
}

}  // namespace voltiris::presentation::ui
