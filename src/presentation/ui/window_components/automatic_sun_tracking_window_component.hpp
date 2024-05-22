//
// Created by sergi on 5/22/2024.
//
#pragma once

#include <string>
#include <vector>

#include <controller/controller_commands_handler.hpp>

#include <presentation/ui/resources.hpp>
#include <presentation/ui/window_components/window_component.hpp>

namespace voltiris::presentation::ui {

class AutomaticSunTrackingWindowComponent : public WindowComponent {
public:
  explicit AutomaticSunTrackingWindowComponent(controller::ControllerCommandsHandler& commands_handler);

  void Render(double scale) override;

  // RENDERING LOGIC

  // DATA
  void OnLatitude(double value) { _latitude = value; }
  void OnLongitude(double value) { _longitude = value; }
  void OnSunAzimuth(double value) { _sun_azimuth = value; }
  void OnSunElevation(double value) { _sun_elevation = value; }
  void OnSunRefAzimuth(double value) { _sun_ref_azimuth = value; }
  void OnSunRefElevation(double value) { _sun_ref_elevation = value; }
  void OnHra(double value) { _hra = value; }
  void OnHrar(double value) { _hrar = value; }
  void OnLocalTime(std::int32_t hours, std::int32_t minutes) {
    _local_time_h = hours;
    _local_time_m = minutes;
  }
  void OnSystemTime(std::int32_t hours, std::int32_t minutes) {
    _system_time_h = hours;
    _system_time_m = minutes;
  }

private:
  // RENDERING LOGIC
  std::int32_t _count_cycles = Resources::kASTCountCyclesDefaultValue;

  // DATA
  double _latitude = Resources::kASTLatitudeDefaultValue;
  double _longitude = Resources::kASTLongitudeDefaultValue;
  std::int32_t _local_time_h = 0;
  std::int32_t _local_time_m = 0;
  std::int32_t _system_time_h = 0;
  std::int32_t _system_time_m = 0;
  double _sun_azimuth = 0;
  double _sun_elevation = 0;
  double _sun_ref_azimuth = 0;
  double _sun_ref_elevation = 0;
  double _hra = 0;
  double _hrar = 0;
};

}  // namespace voltiris::presentation::ui
