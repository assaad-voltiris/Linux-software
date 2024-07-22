#include "sun_math.hpp"

#include <cmath>

namespace voltiris::controller::utils {

double GetTheoreticalAzimuthMm(double az, double a1, double calibration_azimuth_mm) { return -(((az / a1))) + calibration_azimuth_mm; }

double GetTheoreticalElevationMm(double az, double el, double a1, double a2, double a3, double calibration_elevation_mm) {
  return -(((el / a3 - a2 * az / (a1 * a3)))) + calibration_elevation_mm;
}

double GetB(std::int32_t month, std::int32_t day) {
  double d = 30.44 * static_cast<double>(month) + static_cast<double>(day);
  double B = 2.00 * PI * (d - 81.00) / 365.0;
  return B;
}

double GetDeltaRad(double B) {
  double delta = 23.45 * std::sin(B);
  double delta_rad = delta / 360.00 * 2.00 * PI;
  return delta_rad;
}

double GetSolarTimeDec(std::int32_t hour, std::int32_t minute, double longitude, double delta_rad, double B) {
  double EoT = 9.87 * std::sin(2 * B) - 7.53 * std::cos(B) - 1.5 * std::sin(B);
  double time_diff_long_hour = (4.00 * longitude + EoT) / 60.00;
  double time_utc_dec = static_cast<double>(hour) + static_cast<double>(minute) / 60.00;
  double solar_time_dec = time_utc_dec + time_diff_long_hour;
  return solar_time_dec;
}

double GetFHra(double solar_time_dec) { return 15.00 * (solar_time_dec - 12.00); }

double GetRad(double value_deg) { return value_deg / 360.00 * 2.00 * PI; }

double GetDeg(double value_rad) { return value_rad / 2.00 / PI * 360; }

double GetAlphaRad(double delta_rad, double lat_rad, double hra_rad) {
  return std::asin(std::sin(delta_rad) * std::sin(lat_rad) + std::cos(delta_rad) * std::cos(lat_rad) * std::cos(hra_rad));
}

double GetAzimuthRad(double delta_rad, double lat_rad, double hra_rad, double alpha_rad) {
  return std::acos((std::sin(delta_rad) * std::cos(lat_rad) - std::cos(delta_rad) * std::sin(lat_rad) * std::cos(hra_rad)) / std::cos(alpha_rad));  // AZ RAD
}

}  // namespace voltiris::controller::utils