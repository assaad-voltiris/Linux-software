//
// Created by sergi on 5/22/2024.
//

#pragma once

#include <cstdint>
#include <string>

#include <controller/controller_commands_handler.hpp>

#include <presentation/ui/resources.hpp>
#include <presentation/ui/window_components/window_component.hpp>

namespace voltiris::presentation::ui {

class AcceleratedTrackingSettingsWindowComponent : public WindowComponent {
public:
  explicit AcceleratedTrackingSettingsWindowComponent(controller::ControllerCommandsHandler& commands_handler);

  void Render(double scale) override;

  // RENDERING LOGIC

  // DATA
  void OnCycleFrequency(double value);
  void OnStartingHraEnabled(bool value);
  void OnStartingHra(double value);
  void OnAccelerationFactor(std::int32_t value);

private:
  // RENDERING LOGIC
  std::int32_t _cycle_frequency_current_index = 0;

  bool _checkbox_400_0 = true;
  bool _checkbox_400_0_enabled = false;

  bool _checkbox_to_theoretical_position = true;
  bool _checkbox_to_theoretical_position_enabled = false;

  bool _checkbox_90_deg = false;
  bool _checkbox_90_deg_enabled = false;

  bool _checkbox_initial_position = false;
  bool _checkbox_initial_position_enabled = false;

  bool _starting_hra_enabled = false;
  double _starting_hra = Resources::kATSStartingHraDefaultValue;

  std::int32_t _acceleration_factor = 0;

  // DATA
};

}  // namespace voltiris::presentation::ui
