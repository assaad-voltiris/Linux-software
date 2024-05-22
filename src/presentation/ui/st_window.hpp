#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include <imgui.h>

#include <controller/controller_commands_handler.hpp>
#include <controller/controller_update_listener.hpp>

#include <presentation/ui/resources.hpp>
#include <presentation/ui/window_components/accelerated_tracking_settings_window_component.hpp>
#include <presentation/ui/window_components/automatic_sun_tracking_window_component.hpp>
#include <presentation/ui/window_components/calibration_position_window_component.hpp>
#include <presentation/ui/window_components/configuration_window_component.hpp>
#include <presentation/ui/window_components/error_window_component.hpp>
#include <presentation/ui/window_components/manual_reflectors_alignment_window_component.hpp>
#include <presentation/ui/window_components/menu_window_component.hpp>
#include <presentation/ui/window_components/reflectors_data_window_component.hpp>

namespace voltiris::presentation::ui {

using ReflectorState = controller::ReflectorState;

class STWindow : public controller::ControllerUpdateListener {
public:
  explicit STWindow(controller::ControllerCommandsHandler& commands_handler);

  void Render(float scale);

private:
  // ReflectorsDataObserver interface
  void OnStatusUpdate(controller::ControllerStatus new_status) override {}
  void OnError(const std::string& error_string) override { _error_window_component.OnError(error_string); }

  void OnBusListUpdate(const std::vector<std::string>& available_buss) override { _configuration_windows_component.OnBusListUpdate(available_buss); }
  void OnBusConnected() override { _configuration_windows_component.OnConnected(); }
  void OnBusDisconnected() override { _configuration_windows_component.OnDisconnected(); }

  void OnASTLatitude(double value) override { _ast_window_component.OnLatitude(value); }
  void OnASTLongitude(double value) override { _ast_window_component.OnLongitude(value); }
  void OnASTLocalTime(std::int32_t hours, std::int32_t minutes) override { _ast_window_component.OnLocalTime(hours, minutes); }
  void OnASTSystemTime(std::int32_t hours, std::int32_t minutes) override { _ast_window_component.OnSystemTime(hours, minutes); }
  void OnASTSunAzimuth(double value) override { _ast_window_component.OnSunAzimuth(value); }
  void OnASTSunElevation(double value) override { _ast_window_component.OnSunElevation(value); }
  void OnASTSunRefAzimuth(double value) override { _ast_window_component.OnSunRefAzimuth(value); }
  void OnASTSunRefElevation(double value) override { _ast_window_component.OnSunRefElevation(value); }
  void OnASTHra(double value) override { _ast_window_component.OnHra(value); }
  void OnASTHrar(double value) override { _ast_window_component.OnHrar(value); }

  void OnCPAzimuthMotorPosition(double value) override {}
  void OnCPAzimuthAngle(double value) override {}
  void OnCPElevationMotorPosition(double value) override {}
  void OnCPElevationAngle(double value) override {}

  void OnRDReflectorsSize(std::size_t size) override { _rd_window_component.OnReflectorsSize(size); };
  void OnRDReflectorState(std::size_t reflector_index, const ReflectorState& state) override { _rd_window_component.OnReflectorState(reflector_index, state); };

private:
  controller::ControllerCommandsHandler& _commands_handler;

  MenuWindowComponent _menu_window_component;
  ConfigurationWindowComponent _configuration_windows_component;

  AcceleratedTrackingSettingsWindowComponent _atc_window_component;
  ManualReflectorsAlignmentWindowComponent _mra_window_component;
  AutomaticSunTrackingWindowComponent _ast_window_component;
  CalibrationPositionWindowComponent _cp_window_component;
  ReflectorsDataWindowComponent _rd_window_component;

  ErrorWindowComponent _error_window_component;
};

}  // namespace voltiris::presentation::ui
