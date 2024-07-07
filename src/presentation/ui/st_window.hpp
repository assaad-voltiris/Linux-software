#pragma once

#include <array>
#include <cstdint>
#include <mutex>
#include <queue>
#include <string>
#include <vector>

#include <imgui.h>

#include <controller/controller_commands_handler.hpp>
#include <controller/controller_update_listener.hpp>
#include <controller/updates/reflectors_controller_update_visitor.hpp>

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

class STWindow : public controller::ControllerUpdateListener, public controller::ReflectorsControllerUpdateVisitor {
public:
  explicit STWindow(controller::ControllerCommandsHandler& commands_handler, bool is_client_mode);

  bool Init();

  void ProcessUpdates();

  void Render(float scale);
  ~STWindow() override = default;

private:
  // ReflectorsDataObserver interface
  void OnUpdate(std::unique_ptr<controller::ReflectorsControllerUpdate> update) override;

  void ProcessUpdate(const controller::CurrentConfigurationUpdate& update) override;
  void ProcessUpdate(const controller::ASTUpdate& update) override;
  void ProcessUpdate(const controller::BusConnectedUpdate& update) override;
  void ProcessUpdate(const controller::BusDisconnectedUpdate& update) override;
  void ProcessUpdate(const controller::BusListUpdate& update) override;
  void ProcessUpdate(const controller::CPUpdate& update) override;
  void ProcessUpdate(const controller::ReflectorStateUpdate& update) override;
  void ProcessUpdate(const controller::ReflectorsSizeUpdate& update) override;
  void ProcessUpdate(const controller::ErrorUpdate& update) override;
  void ProcessUpdate(const controller::StatusUpdate& update) override;

private:
  controller::ControllerCommandsHandler& _commands_handler;

  std::queue<std::unique_ptr<controller::ReflectorsControllerUpdate>> _updates_queue;
  std::mutex _updates_queue_mutex;

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
