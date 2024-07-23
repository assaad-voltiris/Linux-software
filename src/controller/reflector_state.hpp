#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace voltiris::controller {

struct ReflectorState {
  std::int32_t id;
  std::int32_t com_id;
  std::int32_t line_num;

  double latitude;      // Latitude
  double longitude;     // Longitude
  double hall_spacing;  // Hall spacing mm
  double firmware;      // Firmware

  double a1;  // Mechanical factors a1 of reflector
  double a2;  // Mechanical factors  a2 of reflector
  double a3;  // Mechanical factors  a2 of reflector

  std::int32_t id_wafer;  // unique ID1
  std::int32_t id_die;    // unique ID2

  double actual_position_elevation_mm;        // Current position EL(mm) of reflector
  double actual_position_elevation_deg;       // Current position EL(deg) of reflector
  double actual_position_azimuth_mm;          // Current position AZ(mm) of reflector
  double actual_position_azimuth_deg;         // Current position AZ(deg) of reflector
  double actual_position_elevation_previous;  // Old position EL(mm) of reflector--if reboot
  double actual_position_azimuth_previous;    // Old position AZ(mm) of reflector--if reboot
  bool azimuth_is_max = false;
  bool elevation_is_min = false;
  std::int32_t actual_status_azimuth;    // Current error status of reflector
  std::int32_t actual_status_elevation;  // Current error status of reflector

  double theoretical_position_elevation_mm;        // Current theoretical position EL(mm) of reflector
  double theoretical_position_elevation_deg;       // Current theoretical position EL(deg) of reflector
  double theoretical_position_azimuth_mm;          // Current position AZ(mm) of reflector
  double theoretical_position_azimuth_deg;         // Current position AZ(deg) of reflector
  double theoretical_position_elevation_previous;  // Previous theoretical position EL(mm) of reflector
  double theoretical_position_azimuth_previous;    // Previous theoretical position EL(mm) of reflector

  double calibration_motor_azimuth;    // Calibration position of reflector when 0,400 : morning position
  double calibration_motor_elevation;  // Calibration position of reflector when 0,400 : morning position
  double calibration_elevation_mm;     // Calibration position EL(mm) of reflector
  double calibration_azimuth_mm;       // Calibration position AZ(mm) of reflector
  double calibration_elevation_deg;    // Calibration position EL(degree) of reflector
  double calibration_azimuth_deg;      // Calibration position AZ(degree) of reflector

  double st_spacing = 5.0;

  std::optional<double> should_be_moved_azimuth;
  std::optional<double> should_be_moved_elevation;
  bool should_be_calibrated;

  inline bool operator==(const ReflectorState& rhs) const {
    return id == rhs.id && com_id == rhs.com_id && line_num == rhs.line_num && latitude == rhs.latitude && longitude == rhs.longitude &&
           hall_spacing == rhs.hall_spacing && firmware == rhs.firmware && a1 == rhs.a1 && a2 == rhs.a2 && a3 == rhs.a3 && id_wafer == rhs.id_wafer &&
           id_die == rhs.id_die && actual_position_elevation_mm == rhs.actual_position_elevation_mm &&
           actual_position_elevation_deg == rhs.actual_position_elevation_deg && actual_position_azimuth_mm == rhs.actual_position_azimuth_mm &&
           actual_position_azimuth_deg == rhs.actual_position_azimuth_deg && actual_position_elevation_previous == rhs.actual_position_elevation_previous &&
           actual_position_azimuth_previous == rhs.actual_position_azimuth_previous && azimuth_is_max == rhs.azimuth_is_max &&
           elevation_is_min == rhs.elevation_is_min && actual_status_azimuth == rhs.actual_status_azimuth &&
           actual_status_elevation == rhs.actual_status_elevation && theoretical_position_elevation_mm == rhs.theoretical_position_elevation_mm &&
           theoretical_position_elevation_deg == rhs.theoretical_position_elevation_deg &&
           theoretical_position_azimuth_mm == rhs.theoretical_position_azimuth_mm && theoretical_position_azimuth_deg == rhs.theoretical_position_azimuth_deg &&
           theoretical_position_elevation_previous == rhs.theoretical_position_elevation_previous &&
           theoretical_position_azimuth_previous == rhs.theoretical_position_azimuth_previous && calibration_motor_azimuth == rhs.calibration_motor_azimuth &&
           calibration_motor_elevation == rhs.calibration_motor_elevation && calibration_elevation_mm == rhs.calibration_elevation_mm &&
           calibration_azimuth_mm == rhs.calibration_azimuth_mm && calibration_elevation_deg == rhs.calibration_elevation_deg &&
           calibration_azimuth_deg == rhs.calibration_azimuth_deg && st_spacing == rhs.st_spacing;
  }

  inline bool operator!=(const ReflectorState& rhs) const { return !(*this == rhs); }
};

}  // namespace voltiris::controller