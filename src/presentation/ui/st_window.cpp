#include "st_window.hpp"

#include <imgui.h>

#include <spdlog/spdlog.h>

#include <portable-file-dialogs.h>

namespace voltiris::presentation::ui {

STWindow::STWindow(controller::ControllerCommandsHandler &commands_handler)
    : _commands_handler(commands_handler),
      _menu_window_component(_commands_handler),
      _configuration_windows_component(_commands_handler),
      _atc_window_component(_commands_handler),
      _mra_window_component(_commands_handler),
      _ast_window_component(_commands_handler),
      _cp_window_component(_commands_handler),
      _rd_window_component(_commands_handler),
      _error_window_component(_commands_handler) {
  spdlog::info("Dialogs availability: {}", pfd::settings::available());
}

void STWindow::Render(float scale) {
  ImGui::Begin("Global", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);

  _menu_window_component.Render(scale);
  _configuration_windows_component.Render(scale);

  _atc_window_component.Render(scale);

  ImGui::SameLine();
  _mra_window_component.Render(scale);

  ImGui::SameLine();
  _ast_window_component.Render(scale);

  ImGui::SameLine();
  _cp_window_component.Render(scale);

  _rd_window_component.Render(scale);

  _error_window_component.Render(scale);
  ImGui::End();
}

}  // namespace voltiris::presentation::ui
