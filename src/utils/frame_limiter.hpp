//
// Created by sergi on 7/21/2024.
//

#pragma once

#include <chrono>
#include <cstdint>

namespace voltiris::utils {

class FrameLimiter {
public:
  explicit FrameLimiter(std::size_t fps);

  void StartFrame();
  void LimitFrame();

  [[nodiscard]] std::size_t GetFramesCount() const { return _frames_count; }

private:
  std::size_t _fps;
  std::chrono::nanoseconds _max_frame_time;

  std::size_t _frames_count = 0;

  std::chrono::high_resolution_clock::time_point _last_frame_start_time;
};

}  // namespace voltiris::utils