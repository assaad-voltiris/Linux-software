#include "st_window.hpp"

#include <imgui.h>
#include <portable-file-dialogs.h>
#include <spdlog/spdlog.h>

#include <commands/load_configuration_command.hpp>
#include <commands/values_update_command.hpp>
#include <string>
#include <vector>

namespace voltiris {

namespace {
const char *kACSCycleFrequency[] = {"1", "2", "7", "OFF"};
const char *kACSAccelerationFactor[] = {"1", "4", "5", "6", "7", "8", "10", "12", "18", "24"};
const char *kRCAddress[] = {"COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9", "COM10", "COM11", "COM12", "COM13", "COM14"};
const char *kRCCommId[] = {"Select Com ID"};

ImVec2 GetRealSize(const ImVec2 &original_size, float scale) { return {original_size.x * scale, original_size.y * scale}; }

bool ButtonCenteredOnLine(const char *label, float scale, float alignment = 0.5f) {
  ImGuiStyle &style = ImGui::GetStyle();

  float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
  float avail = ImGui::GetContentRegionAvail().x;

  float off = (avail - size) * alignment;
  if (off > 0.0f) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

  return ImGui::Button(label);
}
}  // namespace

STWindow::STWindow(ReflectorsCommandsObserver &commands_observer) : _commands_observer(commands_observer) {
  std::vector<ValuesUpdateCommand::ValueUpdate> initial_updates;
  initial_updates.emplace_back(ValuesUpdateCommand::ValueId::kCycleFrequency, std::stod(kACSCycleFrequency[_atc_cycle_frequency_selected_i]));
  initial_updates.emplace_back(ValuesUpdateCommand::ValueId::kStartingHraEnabled, _atc_starting_hra_enabled);
  initial_updates.emplace_back(ValuesUpdateCommand::ValueId::kStartingHra, _atc_starting_hra);
  initial_updates.emplace_back(ValuesUpdateCommand::ValueId::kAccelerationFactor, std::stod(kACSAccelerationFactor[_atc_acceleration_factor_i]));
  initial_updates.emplace_back(ValuesUpdateCommand::ValueId::kLatitude, _ast_latitude);
  initial_updates.emplace_back(ValuesUpdateCommand::ValueId::kLongitude, _ast_longitude);
  _commands_observer.OnCommand(std::make_unique<ValuesUpdateCommand>(initial_updates));

  spdlog::info("Dialogs availability: {}", pfd::settings::available());
}

void STWindow::OnError(const std::string &error_string) { _current_error = error_string; }

void STWindow::Render(float scale) {
  ImGui::Begin("Global", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);

  RenderMenu(scale);

  RenderAcceleratedTrackingSettings(scale);
  ImGui::SameLine();
  RenderManualReflectorsAlignment(scale);
  ImGui::SameLine();
  RenderAutomaticSunTracking(scale);
  ImGui::SameLine();
  RenderCalibrationPosition(scale);

  RenderReflectorsData(scale);

  RenderError(scale);

  RenderReflectorsConfiguration(scale);

  ImGui::End();
}

void STWindow::RenderMenu(float scale) {
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      ImGui::MenuItem("New", nullptr, &_menu_file_new);
      ImGui::MenuItem("Open", nullptr, &_menu_file_open);
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Config")) {
      if (ImGui::MenuItem("Reflectors Configuration", nullptr, &_menu_config_reflectors_configuration)) {}
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Log")) {
      ImGui::MenuItem("Log Window", nullptr, &_menu_log_window);
      ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
  }
}

void STWindow::RenderReflectorsConfiguration(float scale) {
  if (_menu_config_reflectors_configuration != _rc_opened) {
    _rc_opened = _menu_config_reflectors_configuration;
    if (_rc_opened) { ImGui::OpenPopup("Reflectors Configuration"); }
  }

  bool temp = true;
  if (ImGui::BeginPopupModal("Reflectors Configuration", &temp, ImGuiWindowFlags_AlwaysAutoResize)) {
    if (ImGui::Button("Select Log Directory")) {
      auto dir = pfd::select_folder("Choose logs directory", pfd::path::home()).result();
      strcpy(_rc_logs_dir, dir.c_str());
    }

    ImGui::SameLine();
    ImGui::InputText("Logs Directory", _rc_logs_dir, IM_ARRAYSIZE(_rc_logs_dir), ImGuiInputTextFlags_ReadOnly);

    ImGui::SeparatorText("Reflectors Configuration");

    if (ImGui::BeginTable("Reflectors Configuration", 5)) {
      ImGui::TableSetupColumn("RS485 Bus");
      ImGui::TableSetupColumn("Address");
      ImGui::TableSetupColumn("Connect");
      ImGui::BeginDisabled();
      ImGui::TableSetupColumn("CommID");
      ImGui::EndDisabled();
      ImGui::TableSetupColumn("CommID");
      ImGui::TableHeadersRow();

      for (std::size_t i = 0; i < 6; ++i) {
        std::string address_name = "##reflector_conf_address" + std::to_string(i);
        std::string connect_name = "##reflector_conf_connect" + std::to_string(i);
        std::string comm_id_1_name = "##reflector_conf_comm_id_1" + std::to_string(i);
        std::string comm_id_2_name = "COMMID##" + std::to_string(i);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("RS485-1");
        ImGui::TableSetColumnIndex(1);
        if (ImGui::BeginCombo(address_name.c_str(), kRCAddress[_rc_address_selected_i[i]], ImGuiComboFlags_WidthFitPreview)) {
          for (std::int32_t j = 0; j < IM_ARRAYSIZE(kRCAddress); j++) {
            const bool is_selected = (_rc_address_selected_i[i] == j);
            if (ImGui::Selectable(kRCAddress[j], is_selected)) { _rc_address_selected_i[i] = j; }
            if (is_selected) { ImGui::SetItemDefaultFocus(); }
          }
          ImGui::EndCombo();
        }
        ImGui::TableSetColumnIndex(2);
        if (ImGui::Checkbox(connect_name.c_str(), &_rc_connected[i]) && &_rc_connected[i]) {}
        ImGui::TableSetColumnIndex(3);
        ImGui::BeginDisabled();
        if (ImGui::BeginCombo(comm_id_1_name.c_str(), kRCCommId[_rc_comm_id_selected_i[i]], ImGuiComboFlags_WidthFitPreview)) {
          for (std::int32_t j = 0; j < IM_ARRAYSIZE(kRCCommId); j++) {
            const bool is_selected = (_rc_comm_id_selected_i[i] == j);
            if (ImGui::Selectable(kRCCommId[j], is_selected)) { _rc_comm_id_selected_i[i] = j; }
            if (is_selected) { ImGui::SetItemDefaultFocus(); }
          }
          ImGui::EndCombo();
        }
        ImGui::EndDisabled();
        ImGui::TableSetColumnIndex(4);
        if (ImGui::Button(comm_id_2_name.c_str())) {
          auto file = pfd::open_file("Choose config file", pfd::path::home(), {"Text Files (.txt .text)", "*.txt *.text"}).result();
          if (!file.empty()) { _commands_observer.OnCommand(std::make_unique<LoadConfigurationCommand>(i, kRCAddress[_rc_address_selected_i[i]], file[0])); }
        }
      }
      ImGui::EndTable();
    }

    ImGui::EndPopup();
  } else {
    _menu_config_reflectors_configuration = false;
  }
}

void STWindow::RenderAcceleratedTrackingSettings(float scale) {
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
  if (ImGui::BeginCombo("##Cycle frequency", kACSCycleFrequency[_atc_cycle_frequency_selected_i], ImGuiComboFlags_WidthFitPreview)) {
    for (std::int32_t i = 0; i < IM_ARRAYSIZE(kACSCycleFrequency); i++) {
      const bool is_selected = (_atc_cycle_frequency_selected_i == i);
      if (ImGui::Selectable(kACSCycleFrequency[i], is_selected)) { _atc_cycle_frequency_selected_i = i; }
      if (is_selected) { ImGui::SetItemDefaultFocus(); }
    }
    ImGui::EndCombo();
  }
  ImGui::EndGroup();

  // Center column
  ImGui::SameLine();
  ImGui::BeginGroup();
  ImGui::Checkbox("400-0", &_atc_checkbox_400_0);
  ImGui::Checkbox("to th. pos.", &_atc_checkbox_to_th_pos);
  ImGui::BeginDisabled(!_atc_checkbox_90_deg_enabled);
  ImGui::Checkbox("Perp. to Sun", &_atc_checkbox_90_deg);
  ImGui::EndDisabled();
  ImGui::BeginDisabled(!_atc_checkbox_init_pos_enabled);
  ImGui::Checkbox("to init pos.", &_atc_checkbox_init_pos);
  ImGui::EndDisabled();
  ImGui::EndGroup();

  // Right column
  ImGui::SameLine();
  ImGui::BeginGroup();
  if (ImGui::Button("Move", GetRealSize({80, 60}, scale))) {}
  ImGui::EndGroup();
  ImGui::EndGroup();

  // Bottom line
  ImGui::BeginGroup();
  if (ImGui::Checkbox("Enable Starting Hra", &_atc_starting_hra_enabled)) {
    _commands_observer.OnCommand(std::make_unique<ValuesUpdateCommand>(
        std::vector<ValuesUpdateCommand::ValueUpdate>{{ValuesUpdateCommand::ValueId::kStartingHraEnabled, _atc_starting_hra_enabled}}));
  }
  ImGui::BeginDisabled(!_atc_starting_hra_enabled);
  if (ImGui::InputDouble("Starting Hra", &_atc_starting_hra)) {
    _commands_observer.OnCommand(
        std::make_unique<ValuesUpdateCommand>(std::vector<ValuesUpdateCommand::ValueUpdate>{{ValuesUpdateCommand::ValueId::kStartingHra, _atc_starting_hra}}));
  }
  ImGui::EndDisabled();
  if (ImGui::BeginCombo("Acceleration Factor", kACSAccelerationFactor[_atc_acceleration_factor_i], ImGuiComboFlags_WidthFitPreview)) {
    for (std::int32_t i = 0; i < IM_ARRAYSIZE(kACSAccelerationFactor); i++) {
      const bool is_selected = (_atc_acceleration_factor_i == i);
      if (ImGui::Selectable(kACSAccelerationFactor[i], is_selected)) { _atc_acceleration_factor_i = i; }
      if (is_selected) { ImGui::SetItemDefaultFocus(); }
    }
    ImGui::EndCombo();
  }
  ImGui::EndGroup();

  ImGui::EndChild();
  ImGui::EndGroup();
}

void STWindow::RenderManualReflectorsAlignment(float scale) {
  // General group for AcceleratedTrackingSettings
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
  ImGui::InputDouble("Step size (°)", &_mra_step_size);
  ImGui::InputText("Reflectors To Move", _mra_reflectors_to_move, IM_ARRAYSIZE(_mra_reflectors_to_move));
  ImGui::EndGroup();

  ImGui::EndChild();
  ImGui::EndGroup();
}

void STWindow::RenderAutomaticSunTracking(float scale) {
  // General group for AutomaticSunTracking
  ImGui::BeginGroup();

  ImGui::BeginChild("Automatic Sun Tracking", {}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle);
  ImGui::SeparatorText("Automatic Sun Tracking");

  ImGui::InputDouble("Latitude (°)", &_ast_latitude);
  ImGui::InputDouble("Longitude (°)", &_ast_longitude);

  if (ImGui::Button("START TRACKING")) {}
  ImGui::SameLine();
  if (ImGui::Button("STOP TRACKING")) {}

  if (ImGui::Button("Count Cycles")) {}
  ImGui::SameLine();
  ImGui::BeginDisabled();
  ImGui::InputInt("##count_cycles", &_ast_count_cycles);
  ImGui::EndDisabled();

  ImGui::Text("LT %02dh%02dm, ST %02dh%02dm", _ast_local_time_h, _ast_local_time_m, _ast_system_time_h, _ast_system_time_m);
  ImGui::Text("Sun AZ %3.3f° / EL %3.3f°", _ast_sun_azimuth, _ast_sun_elevation);
  ImGui::Text("Ref AZ %3.3f° / EL %3.3f°", _ast_sun_ref_azimuth, _ast_sun_ref_elevation);
  ImGui::Text("hra %3.3f° / hrar %3.3f°", _ast_hra, _ast_hrar);

  ImGui::EndChild();
  ImGui::EndGroup();
}

void STWindow::RenderCalibrationPosition(float scale) {
  // General group for CalibrationPosition
  ImGui::BeginGroup();

  ImGui::BeginChild("Calibration Position", {}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle);
  ImGui::SeparatorText("Calibration Position");

  ImGui::InputDouble("AZIMUTH Motor position (mm)", &_cp_azimuth_motor_pos);
  ImGui::InputDouble("AZIMUTH Angle (°)", &_cp_azimuth_angle);

  ImGui::InputDouble("ELEVATION Motor position (mm)", &_cp_elevation_motor_pos);
  ImGui::InputDouble("ELEVATION Angle (°)", &_cp_elevation_angle);

  ImGui::Text("MOTOR POSITION: RefAZ=%d/RefEL=%d", 400, 200);

  ImGui::EndChild();
  ImGui::EndGroup();
}

void STWindow::RenderReflectorsData(float scale) {
  // General group for ReflectorsData
  ImGui::BeginGroup();

  ImGui::BeginChild("Reflectors Data", {}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle);
  ImGui::SeparatorText("Reflectors Data");

  // Left buttons
  ImGui::BeginGroup();
  if (ImGui::Button("INIT")) {}
  if (ImGui::Button("READ")) {}
  if (ImGui::Button("FLASH")) {}
  if (ImGui::Button("REBOOT")) {}
  if (ImGui::Button("STOP csv+log")) {}
  if (ImGui::Button(_rd_is_deg ? "Turn to MM" : "Turn to DEG")) { _rd_is_deg = !_rd_is_deg; }
  ImGui::EndGroup();

  ImGui::SameLine();

  if (ImGui::BeginTable("Reflectors data", 12)) {
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn(_rd_is_deg ? "Pos AZ (°)" : "Pos AZ (mm)");
    ImGui::TableSetupColumn("Status AZ");
    ImGui::TableSetupColumn(_rd_is_deg ? "Pos EL (°)" : "Pos EL (mm)");
    ImGui::TableSetupColumn("Status EL");
    ImGui::TableSetupColumn("Reflector Info");
    ImGui::TableSetupColumn(_rd_is_deg ? "Th. RPosAZ (°)" : "Th. RPosAZ (mm)");
    ImGui::TableSetupColumn(_rd_is_deg ? "Th. RPosEL (°)" : "Th. RPosEL (mm)");
    ImGui::TableSetupColumn("PosAZCtrl");
    ImGui::TableSetupColumn("PosELCtrl");
    ImGui::TableSetupColumn("SET");
    ImGui::TableSetupColumn("GO");
    ImGui::TableHeadersRow();

    for (std::size_t i = 0; i < _rd_data.size(); ++i) {
      std::string ctr_az_name = "##reflector_data_ctr_az" + std::to_string(i);
      std::string ctr_el_name = "##reflector_data_ctr_el" + std::to_string(i);

      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::Text("REF-%zu", i);
      ImGui::TableSetColumnIndex(1);
      ImGui::Text("%d", (int)std::round(_rd_is_deg ? _rd_data[i].pos_az_deg : _rd_data[i].pos_az_mm));
      ImGui::TableSetColumnIndex(2);
      ImGui::Text("%d", (int)_rd_data[i].status_az);
      ImGui::TableSetColumnIndex(3);
      ImGui::Text("%d", (int)std::round(_rd_is_deg ? _rd_data[i].pos_el_deg : _rd_data[i].pos_el_mm));
      ImGui::TableSetColumnIndex(4);
      ImGui::Text("%d", (int)_rd_data[i].status_el);
      ImGui::TableSetColumnIndex(5);
      ImGui::Text("%d", (int)_rd_data[i].reflector_info);
      ImGui::TableSetColumnIndex(6);
      ImGui::Text("%d", (int)std::round(_rd_is_deg ? _rd_data[i].th_r_pos_az_deg : _rd_data[i].th_r_pos_az_mm));
      ImGui::TableSetColumnIndex(7);
      ImGui::Text("%d", (int)std::round(_rd_is_deg ? _rd_data[i].th_r_pos_el_deg : _rd_data[i].th_r_pos_el_mm));
      ImGui::TableSetColumnIndex(8);
      ImGui::InputText(ctr_az_name.c_str(), _rd_data[i].pos_az_ctrl, IM_ARRAYSIZE(_rd_data[i].pos_az_ctrl));
      ImGui::TableSetColumnIndex(9);
      ImGui::InputText(ctr_el_name.c_str(), _rd_data[i].pos_el_ctrl, IM_ARRAYSIZE(_rd_data[i].pos_el_ctrl));
      ImGui::TableSetColumnIndex(10);
      if (ImGui::Button("SET")) {}
      ImGui::TableSetColumnIndex(11);
      if (ImGui::Button("GO")) {}
    }
    ImGui::EndTable();
  }

  ImGui::EndChild();
  ImGui::EndGroup();
}

void STWindow::RenderError(float scale) {
  if (!_current_error.empty() && !_error_popup_opened) {
    _error_popup_opened = true;
    spdlog::error("Error message: {}", _current_error);
    ImGui::OpenPopup("Warning Message");
  }

  bool temp = true;
  if (ImGui::BeginPopupModal("Warning Message", &temp, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Error happened: %s", _current_error.c_str());
    if (ButtonCenteredOnLine("OK", 0.5f)) { ImGui::CloseCurrentPopup(); }
    ImGui::EndPopup();
  } else {
    _current_error.clear();
    _error_popup_opened = false;
  }
}

}  // namespace voltiris
