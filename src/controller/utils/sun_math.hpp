#pragma once

#include <cstdint>

namespace voltiris::controller::utils {

#define PI 3.14159265358979323846 /* pi */

double GetTheoreticalAzimuthMm(double az, double a1, double calibration_azimuth_mm);

double GetTheoreticalElevationMm(double az, double el, double a1, double a2, double a3, double calibration_elevation_mm);

double GetB(std::int32_t month, std::int32_t day);

double GetDeltaRad(double B);

double GetSolarTimeDec(std::int32_t month, std::int32_t day, double longitude, double delta_rad, double B);

double GetFHra(double solar_time_dec);

double GetRad(double value_deg);

double GetDeg(double value_rad);

double GetAlphaRad(double delta_rad, double lat_rad, double hra_rad);

double GetAzimuthRad(double delta_rad, double lat_rad, double hra_rad, double alpha_rad);

}  // namespace voltiris::controller::utils