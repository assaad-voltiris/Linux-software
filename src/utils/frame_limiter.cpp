//
// Created by sergi on 7/21/2024.
//

#include "frame_limiter.hpp"

#include <thread>

namespace voltiris::utils {

FrameLimiter::FrameLimiter(std::size_t fps)
    : _fps(fps),
      _max_frame_time(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)) / _fps),
      _last_frame_start_time(std::chrono::high_resolution_clock::now()) {}

void FrameLimiter::StartFrame() { _last_frame_start_time = std::chrono::high_resolution_clock::now(); }

void FrameLimiter::LimitFrame() {
  auto frame_time = std::chrono::high_resolution_clock::now() - _last_frame_start_time;
  auto sleep_time = _max_frame_time - frame_time;

  if (sleep_time.count() > 0) { std::this_thread::sleep_for(sleep_time); }

  ++_frames_count;
}

}  // namespace voltiris::utils
