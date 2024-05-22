//
// Created by sergi on 5/22/2024.
//

#pragma once

#include <string>
#include <vector>

#include <controller/controller_commands_handler.hpp>
#include <controller/reflector_state.hpp>

#include <presentation/ui/resources.hpp>
#include <presentation/ui/window_components/window_component.hpp>

namespace voltiris::presentation::ui {

class ReflectorsDataWindowComponent : public WindowComponent {
public:
  explicit ReflectorsDataWindowComponent(controller::ControllerCommandsHandler& commands_handler) : WindowComponent(commands_handler) {}

  void Render(double scale) override;

  // RENDERING LOGIC

  // DATA
  void OnReflectorsSize(std::size_t size) {
    _reflectors_data.clear();
    _reflectors_data.resize(size);

    _reflectors_control_data.clear();
    _reflectors_control_data.resize(size);
  };
  void OnReflectorState(std::size_t reflector_index, const controller::ReflectorState& new_state) { _reflectors_data[reflector_index] = new_state; }

private:
  // RENDERING LOGIC
  bool _show_in_deg = false;

  struct ReflectorsControlValues {
    char position_azimuth[32] = "";
    char position_elevation[32] = "";
  };
  std::vector<ReflectorsControlValues> _reflectors_control_data;

  // DATA
  std::vector<controller::ReflectorState> _reflectors_data;
};

}  // namespace voltiris::presentation::ui
