//
// Created by sergi on 5/22/2024.
//

#include "menu_window_component.hpp"

#include <imgui.h>

#include <presentation/ui/window_components/configuration_window_component.hpp>

namespace voltiris::presentation::ui {

void MenuWindowComponent::Render(double scale) {
  bool open_configuration = false;

  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      ImGui::MenuItem("New", nullptr);
      ImGui::MenuItem("Open", nullptr);
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Config")) {
      if (ImGui::MenuItem("Reflectors Configuration", nullptr)) { open_configuration = true; }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Log")) {
      ImGui::MenuItem("Log Window", nullptr);
      ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
  }

  if (open_configuration) { ImGui::OpenPopup(ConfigurationWindowComponent::GetPopupName()); }
}

}  // namespace voltiris::presentation::ui
