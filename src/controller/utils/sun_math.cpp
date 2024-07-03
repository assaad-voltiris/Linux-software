#include "sun_math.hpp"

#include <cmath>

namespace voltiris::controller::utils {

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

double GetSolarTimeDec(std::int32_t month, std::int32_t day, double longitude, double delta_rad, double B) {
  double EoT = 9.87 * std::sin(2 * B) - 7.53 * std::cos(B) - 1.5 * std::sin(B);
  double time_diff_long_hour = (4.00 * longitude + EoT) / 60.00;
  double time_utc_dec = static_cast<double>(month) + static_cast<double>(day) / 60.00;
  double solar_time_dec = time_utc_dec + time_diff_long_hour;
  return solar_time_dec;
}

double GetFHra(double solar_time_dec) { return 15.00 * (solar_time_dec - 12.00); }

}  // namespace voltiris::controller::utils