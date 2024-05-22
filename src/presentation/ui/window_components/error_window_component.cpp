//
// Created by sergi on 5/22/2024.
//

#include "error_window_component.hpp"

#include <imgui.h>

namespace voltiris::presentation::ui {

namespace {

bool ButtonCenteredOnLine(const char *label, float scale, float alignment = 0.5f) {
  ImGuiStyle &style = ImGui::GetStyle();

  float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
  float avail = ImGui::GetContentRegionAvail().x;

  float off = (avail - size) * alignment;
  if (off > 0.0f) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

  return ImGui::Button(label);
}

}  // namespace

void ErrorWindowComponent::Render(double scale) {
  if (_show_popup) {
    ImGui::OpenPopup("Warning Message");
    _show_popup = false;
  }

  // Temp variable to get cross on popup showed
  bool temp_show_cross = true;

  if (ImGui::BeginPopupModal("Warning Message", &temp_show_cross, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Error happened: \"%s\"", _current_error.c_str());
    if (ButtonCenteredOnLine("OK", 0.5f)) { ImGui::CloseCurrentPopup(); }
    ImGui::EndPopup();
  }
}

}  // namespace voltiris::presentation::ui
