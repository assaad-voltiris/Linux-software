//
// Created by sergi on 5/22/2024.
//

#include "reflectors_data_window_component.hpp"

#include <cmath>

#include <controller/commands/initialization_command.hpp>
#include <controller/commands/read_command.hpp>

namespace voltiris::presentation::ui {

void ReflectorsDataWindowComponent::Render(double scale) {
  // General group for ReflectorsData
  ImGui::BeginGroup();

  ImGui::BeginChild("Reflectors Data", {}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle);
  ImGui::SeparatorText("Reflectors Data");

  // Left buttons
  ImGui::BeginGroup();
  if (ImGui::Button("INIT")) { SendCommand(std::make_unique<controller::InitializationCommand>()); }
  if (ImGui::Button("READ")) { SendCommand(std::make_unique<controller::ReadCommand>()); }
  if (ImGui::Button("FLASH")) {}
  if (ImGui::Button("REBOOT")) {}
  if (ImGui::Button("STOP csv+log")) {}
  if (ImGui::Button(_show_in_deg ? "Turn to MM" : "Turn to DEG")) { _show_in_deg = !_show_in_deg; }
  ImGui::EndGroup();

  ImGui::SameLine();

  if (ImGui::BeginTable("Reflectors data", 12)) {
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn(_show_in_deg ? "Pos AZ (°)" : "Pos AZ (mm)");
    ImGui::TableSetupColumn("Status AZ");
    ImGui::TableSetupColumn(_show_in_deg ? "Pos EL (°)" : "Pos EL (mm)");
    ImGui::TableSetupColumn("Status EL");
    ImGui::TableSetupColumn("Reflector Info");
    ImGui::TableSetupColumn(_show_in_deg ? "Th. RPosAZ (°)" : "Th. RPosAZ (mm)");
    ImGui::TableSetupColumn(_show_in_deg ? "Th. RPosEL (°)" : "Th. RPosEL (mm)");
    ImGui::TableSetupColumn("PosAZCtrl");
    ImGui::TableSetupColumn("PosELCtrl");
    ImGui::TableSetupColumn("SET");
    ImGui::TableSetupColumn("GO");
    ImGui::TableHeadersRow();

    for (std::size_t i = 0; i < _reflectors_data.size(); ++i) {
      std::string ctr_az_name = "##reflector_data_ctr_az" + std::to_string(i);
      std::string ctr_el_name = "##reflector_data_ctr_el" + std::to_string(i);

      const auto& reflector_data = _reflectors_data[i];

      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::Text("REF-%d", _reflectors_data[i].id);
      ImGui::TableSetColumnIndex(1);
      ImGui::Text("%d", (int)std::round(_show_in_deg ? reflector_data.actual_position_azimuth_deg : reflector_data.actual_position_azimuth_mm));
      ImGui::TableSetColumnIndex(2);
      ImGui::Text("%d", (int)reflector_data.actual_status_azimuth);
      ImGui::TableSetColumnIndex(3);
      ImGui::Text("%d", (int)std::round(_show_in_deg ? reflector_data.actual_position_elevation_deg : reflector_data.actual_position_elevation_mm));
      ImGui::TableSetColumnIndex(4);
      ImGui::Text("%d", (int)reflector_data.actual_status_elevation);
      ImGui::TableSetColumnIndex(5);
      // ImGui::Text("%d", (int)reflector_data.reflector_info);
      ImGui::Text("%d", 0);
      ImGui::TableSetColumnIndex(6);
      ImGui::Text("%d", (int)std::round(_show_in_deg ? reflector_data.theoretical_position_azimuth_deg : reflector_data.theoretical_position_azimuth_mm));
      ImGui::TableSetColumnIndex(7);
      ImGui::Text("%d", (int)std::round(_show_in_deg ? reflector_data.theoretical_position_elevation_deg : reflector_data.theoretical_position_elevation_mm));
      ImGui::TableSetColumnIndex(8);
      ImGui::InputText(ctr_az_name.c_str(), _reflectors_control_data[i].position_azimuth, IM_ARRAYSIZE(_reflectors_control_data[i].position_azimuth));
      ImGui::TableSetColumnIndex(9);
      ImGui::InputText(ctr_el_name.c_str(), _reflectors_control_data[i].position_elevation, IM_ARRAYSIZE(_reflectors_control_data[i].position_elevation));
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

}  // namespace voltiris::presentation::ui
