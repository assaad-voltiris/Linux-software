#pragma once

#include <optional>

#include <controller/commands/reflectors_controller_command_visitor.hpp>
#include <controller/controller_commands_handler.hpp>
#include <controller/controller_update_listener.hpp>
#include <controller/utils/changeable.hpp>

namespace voltiris::controller {

struct ReflectorsControllerInternalState {
  // Position
  double latitude = 0;
  double longitude = 0;

  // Configuration
  double starting_hra = -1;
  std::int32_t acceleration_factor = 1;
  std::chrono::steady_clock::time_point start_time;
  // Temp configuration updates
  std::optional<double> new_hra;
  std::optional<std::int32_t> new_acceleration_factor;

  double cycles_frequency = 1;

  // Status
  bool is_tracking = false;

  // Night
  bool is_night;
  bool is_night_returning;
  bool is_night_returned;
};

struct ReflectorsControllerIterationState {
  double latitude = 0;
  double longitude = 0;
  std::int32_t local_hours = 0;
  std::int32_t local_minutes = 0;
  std::int32_t sun_hours = 0;
  std::int32_t sun_minutes = 0;
  double delta_rad;
  double lat_rad;
  double sun_azimuth = 0;
  double sun_elevation = 0;
  double sun_ref_azimuth = 0;
  double sun_ref_elevation = 0;
  double hra = 0;
  double reflector_hra = 0;

  inline bool operator==(const ReflectorsControllerIterationState& rhs) const {
    return latitude == rhs.latitude && longitude == rhs.longitude && local_hours == rhs.local_hours && local_minutes == rhs.local_minutes &&
           sun_hours == rhs.sun_hours && sun_minutes == rhs.sun_minutes && sun_azimuth == rhs.sun_azimuth && sun_elevation == rhs.sun_elevation &&
           sun_ref_azimuth == rhs.sun_ref_azimuth && sun_ref_elevation == rhs.sun_ref_elevation && hra == rhs.hra && reflector_hra == rhs.reflector_hra &&
           delta_rad == rhs.delta_rad && lat_rad == rhs.lat_rad;
  }

  inline bool operator!=(const ReflectorsControllerIterationState& rhs) const { return !(*this == rhs); }
};

}  // namespace voltiris::controller