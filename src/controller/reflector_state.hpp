#pragma once

#include <cstdint>
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
  double azimuth_is_max = false;
  double elevation_is_min = false;
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
};

}  // namespace voltiris::controller