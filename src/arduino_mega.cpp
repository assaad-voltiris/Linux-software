#include "arduino_mega.hpp"

#include <cmath>

namespace voltiris {

double ArduinoMega::ConvertAZ_Mm2Deg(double mm_value) const {
  double delta_azimuth_mm = mm_value - reflector_calibration_azimuth_mm;
  double delta_azimuth_deg = -(delta_azimuth_mm * reflector_a1);
  double deg_value = delta_azimuth_deg + reflector_calibration_azimuth_deg;

  return std::fmod(deg_value, 360);
}

double ArduinoMega::ConvertAZ_Deg2Mm(double deg_value) const {
  double delta_azimuth_deg = deg_value - reflector_calibration_azimuth_deg;
  double delta_azimuth_mm = -(delta_azimuth_deg / reflector_a1);
  double mm_value = delta_azimuth_mm + reflector_calibration_azimuth_mm;

  return mm_value;
}

double ArduinoMega::ConvertEL_Mm2Deg(double mm_value) const {
  double deg_value = reflector_a3 * (reflector_calibration_elevation_mm - reflector_actual_position_elevation) +
                     reflector_a2 * (reflector_calibration_azimuth_mm - reflector_actual_position_azimuth) + reflector_calibration_elevation_deg;

  return deg_value;
}

double ArduinoMega::ConvertEL_Deg2Mm(double deg_value) const {
  double delta_azimuth_mm = reflector_actual_position_azimuth - reflector_calibration_azimuth_mm;
  double delta_elevation_deg = deg_value - reflector_calibration_elevation_deg;
  double delta_elevation_mm = -(delta_elevation_deg / reflector_a3) - (reflector_a2 / reflector_a3) * (delta_azimuth_mm);

  return delta_elevation_mm + reflector_calibration_elevation_mm;
}

double ArduinoMega::ConvertEL_Mm2DegTh(double mm_value) const {
  double deg_value = reflector_a3 * (reflector_calibration_elevation_mm - reflector_th_position_elevation) +
                     reflector_a2 * (reflector_calibration_azimuth_mm - reflector_th_position_azimuth) + reflector_calibration_elevation_deg;

  return deg_value;
}

}  // namespace voltiris
