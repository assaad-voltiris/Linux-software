#pragma once

#include <controller/reflector_state.hpp>

namespace voltiris::controller::utils {

double ConvertAzimuth_Mm2Deg(const ReflectorState& reflector, double mm_value);

double ConvertAzimuth_Deg2Mm(const ReflectorState& reflector, double deg_value);

double ConvertElevation_Mm2Deg(const ReflectorState& reflector, double mm_value);

double ConvertElevation_Deg2Mm(const ReflectorState& reflector, double deg_value);

double ConvertElevation_Mm2DegTh(const ReflectorState& reflector, double mm_value);

}  // namespace voltiris::controller::utils