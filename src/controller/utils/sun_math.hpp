#pragma once

#include <cstdint>

namespace voltiris::controller::utils {

double GetB(std::int32_t month, std::int32_t day);

double GetDeltaRad(double B);

double GetSolarTimeDec(std::int32_t month, std::int32_t day, double longitude, double delta_rad, double B);

double GetFHra(double solar_time_dec);

}  // namespace voltiris::controller::utils