#pragma once

#include <ctime>

namespace voltiris::controller::utils {

time_t GetCurrentTimePoint();

tm GetUTCTime(time_t time_point);
tm GetLocalTime(time_t time_point);

}  // namespace voltiris::controller::utils