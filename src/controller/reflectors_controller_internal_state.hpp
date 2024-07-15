#pragma once

#include <array>
#include <atomic>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

#include <controller/commands/reflectors_controller_command_visitor.hpp>
#include <controller/controller_commands_handler.hpp>
#include <controller/controller_update_listener.hpp>
#include <controller/utils/changeable.hpp>

namespace voltiris::controller {

struct ReflectorsControllerInternalState {
  double cycles_frequency = 1;
  bool hra_enabled = false;
  double hra = 200;
  double hra_input = 0;
  double acceleration_factor = 1;

  double latitude = 0;
  double longitude = 0;

  // Old
  double temp_hra_old = 200;
  double hra_old = 200;
  double hrar_old = 0;
  double acceleration_factor_old = 0;

  bool is_tracking = false;
};

struct ReflectorsControllerIterationState {
  double latitude = 0;
  double longitude = 0;
  std::int32_t local_hours = 0;
  std::int32_t local_minutes = 0;
  std::int32_t system_hours = 0;
  std::int32_t system_minutes = 0;
  double sun_azimuth = 0;
  double sun_elevation = 0;
  double sun_ref_azimuth = 0;
  double sun_ref_elevation = 0;
  double hra = 0;
  double hrar = 0;

  inline bool operator==(const ReflectorsControllerIterationState& rhs) const {
    return latitude == rhs.latitude && longitude == rhs.longitude && local_hours == rhs.local_hours && local_minutes == rhs.local_minutes &&
           system_hours == rhs.system_hours && system_minutes == rhs.system_minutes && sun_azimuth == rhs.sun_azimuth && sun_elevation == rhs.sun_elevation &&
           sun_ref_azimuth == rhs.sun_ref_azimuth && sun_ref_elevation == rhs.sun_ref_elevation && hra == rhs.hra && hrar == rhs.hrar;
  }

  inline bool operator!=(const ReflectorsControllerIterationState& rhs) const { return !(*this == rhs); }
};

}  // namespace voltiris::controller