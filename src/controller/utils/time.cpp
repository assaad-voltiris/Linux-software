#include "time.hpp"

namespace voltiris::controller::utils {

time_t GetCurrentTimePoint() {
  time_t time_t;
  time(&time_t);

  return time_t;
}

tm GetUTCTime(time_t time_point) {
  tm *utc_time{};
  utc_time = gmtime(&time_point);
  return *utc_time;
}

tm GetLocalTime(time_t time_point){
  tm *local_time{};
  local_time = localtime(&time_point);
  return *local_time;
}

}  // namespace voltiris::controller::utils