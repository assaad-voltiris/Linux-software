#include "st_window.hpp"

#include <imgui.h>

#include <spdlog/spdlog.h>

#include <portable-file-dialogs.h>

#include <controller/updates/ast_update.hpp>
#include <controller/updates/bus_connected_update.hpp>
#include <controller/updates/bus_disconnected_update.hpp>
#include <controller/updates/bus_list_update.hpp>
#include <controller/updates/cp_update.hpp>
#include <controller/updates/current_configuration_update.hpp>
#include <controller/updates/error_update.hpp>
#include <controller/updates/reflector_state_update.hpp>
#include <controller/updates/reflectors_size_update.hpp>
#include <controller/updates/status_update.hpp>

#include <controller/commands/request_configuration_command.hpp>

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

bool STWindow::Init() {
  _commands_handler.OnCommand(std::make_unique<controller::RequestConfigurationCommand>());
  return true;
}

void STWindow::ProcessUpdates() {
  std::unique_ptr<controller::ReflectorsControllerUpdate> command;

  std::unique_lock<std::mutex> lock(_updates_queue_mutex);
  while (!_updates_queue.empty()) {
    command = std::move(_updates_queue.front());
    _updates_queue.pop();

    lock.unlock();

    command->Visit(*this);

    lock.lock();
  }
}

void STWindow::OnUpdate(std::unique_ptr<controller::ReflectorsControllerUpdate> update) {
  spdlog::info("New update received.");
  std::lock_guard<std::mutex> lock(_updates_queue_mutex);
  _updates_queue.push(std::move(update));
}

void STWindow::ProcessUpdate(const controller::CurrentConfigurationUpdate &update) {
  spdlog::debug("CurrentConfigurationUpdate received.");
  _atc_window_component.OnAccelerationFactor(update.GetAccelerationFactor());
  _atc_window_component.OnCycleFrequency(update.GetCycleFrequency());
  _atc_window_component.OnStartingHra(update.GetStartingHra());
  _atc_window_component.OnStartingHraEnabled(update.GetStartingHraEnabled());
  _ast_window_component.OnLatitude(update.GetLatitude());
  _ast_window_component.OnLongitude(update.GetLongitude());
}

void STWindow::ProcessUpdate(const controller::ASTUpdate &update) {
  spdlog::debug("ASTUpdate received.");
  _ast_window_component.OnLatitude(update.GetLatitude());
  _ast_window_component.OnLongitude(update.GetLongitude());
  _ast_window_component.OnLocalTime(update.GetLocalHours(), update.GetLocalMinutes());
  _ast_window_component.OnSystemTime(update.GetSystemHours(), update.GetSystemMinutes());
  _ast_window_component.OnSunAzimuth(update.GetSunAzimuth());
  _ast_window_component.OnSunElevation(update.GetSunElevation());
  _ast_window_component.OnSunRefAzimuth(update.GetSunRefAzimuth());
  _ast_window_component.OnSunRefElevation(update.GetSunRefElevation());
  _ast_window_component.OnHra(update.GetHra());
  _ast_window_component.OnHrar(update.GetHrar());
}

void STWindow::ProcessUpdate(const controller::BusConnectedUpdate &update) {
  spdlog::debug("BusConnectedUpdate received.");
  _configuration_windows_component.OnConnected();
}

void STWindow::ProcessUpdate(const controller::BusDisconnectedUpdate &update) {
  spdlog::debug("BusDisconnectedUpdate received.");
  _configuration_windows_component.OnDisconnected();
}

void STWindow::ProcessUpdate(const controller::BusListUpdate &update) {
  spdlog::debug("BusListUpdate received.");
  _configuration_windows_component.OnBusListUpdate(update.GetAvailableBuss());
}

void STWindow::ProcessUpdate(const controller::CPUpdate &update) { spdlog::debug("CPUpdate received."); }

void STWindow::ProcessUpdate(const controller::ReflectorStateUpdate &update) {
  spdlog::debug("ReflectorStateUpdate received.");
  _rd_window_component.OnReflectorState(update.GetReflectorIndex(), update.GetReflectorState());
}

void STWindow::ProcessUpdate(const controller::ReflectorsSizeUpdate &update) {
  spdlog::debug("ReflectorsSizeUpdate received.");
  _rd_window_component.OnReflectorsSize(update.GetSize());
}

void STWindow::ProcessUpdate(const controller::ErrorUpdate &update) {
  spdlog::debug("ErrorUpdate received.");
  _error_window_component.OnError(update.GetError());
}

void STWindow::ProcessUpdate(const controller::StatusUpdate &update) { spdlog::debug("StatusUpdate received."); }

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
