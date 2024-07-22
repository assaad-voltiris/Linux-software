//
// Created by sergi on 5/22/2024.
//

#pragma once

#include <string>
#include <vector>

#include <controller/controller_commands_handler.hpp>

#include <presentation/ui/window_components/window_component.hpp>

namespace voltiris::presentation::ui {

class ConfigurationWindowComponent : public WindowComponent {
public:
  static const char* GetPopupName();

  explicit ConfigurationWindowComponent(controller::ControllerCommandsHandler& commands_handler, bool send_file_content)
      : WindowComponent(commands_handler), _send_configuration_file_content(send_file_content) {}

  void Render(double scale) override;

  // RENDERING LOGIC

  // DATA
  inline void OnConnected() { _is_connected = true; };
  inline void OnDisconnected() { _is_connected = false; };

  void OnBusListUpdate(const std::vector<std::string>& available_buss);

private:
  // RENDERING LOGIC
  char _rc_logs_dir[256] = "";

  std::vector<std::string> _available_buss;
  std::size_t _available_buss_current_index = 0;

  bool _send_configuration_file_content = false;

  // DATA
  bool _is_connected = false;
};

}  // namespace voltiris::presentation::ui
