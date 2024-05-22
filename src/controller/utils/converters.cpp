#include "converters.hpp"

#include <cmath>

namespace voltiris::controller::utils {

double ConvertAzimuth_Mm2Deg(const ReflectorState& reflector, double mm_value) {
  double delta_azimuth_mm = mm_value - reflector.calibration_azimuth_mm;
  double delta_azimuth_deg = -(delta_azimuth_mm * reflector.a1);
  double deg_value = delta_azimuth_deg + reflector.calibration_azimuth_deg;

  return std::fmod(deg_value, 360);
}

double ConvertAzimuth_Deg2Mm(const ReflectorState& reflector, double deg_value) {
  double delta_azimuth_deg = deg_value - reflector.calibration_azimuth_deg;
  double delta_azimuth_mm = -(delta_azimuth_deg / reflector.a1);
  double mm_value = delta_azimuth_mm + reflector.calibration_azimuth_mm;

  return mm_value;
}

double ConvertElevation_Mm2Deg(const ReflectorState& reflector, double mm_value) {
  double deg_value = reflector.a3 * (reflector.calibration_elevation_mm - reflector.actual_position_elevation_mm) +
                     reflector.a2 * (reflector.calibration_azimuth_mm - reflector.actual_position_azimuth_mm) + reflector.calibration_elevation_deg;

  return deg_value;
}

double ConvertElevation_Deg2Mm(const ReflectorState& reflector, double deg_value) {
  double delta_azimuth_mm = reflector.actual_position_azimuth_mm - reflector.calibration_azimuth_mm;
  double delta_elevation_deg = deg_value - reflector.calibration_elevation_deg;
  double delta_elevation_mm = -(delta_elevation_deg / reflector.a3) - (reflector.a2 / reflector.a3) * (delta_azimuth_mm);

  return delta_elevation_mm + reflector.calibration_elevation_mm;
}

double ConvertElevation_Mm2DegTh(const ReflectorState& reflector, double mm_value) {
  double deg_value = reflector.a3 * (reflector.calibration_elevation_mm - reflector.theoretical_position_elevation_mm) +
                     reflector.a2 * (reflector.calibration_azimuth_mm - reflector.theoretical_position_azimuth_mm) + reflector.calibration_elevation_deg;

  return deg_value;
}

}  // namespace voltiris::controller::utils