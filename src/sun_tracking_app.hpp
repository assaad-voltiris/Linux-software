#pragma once

#include <controller/reflectors_controller.hpp>
#include <presentation/ui/renderer.hpp>
#include <presentation/ui/st_window.hpp>

namespace voltiris {

class SunTrackingApp {
public:
  virtual ~SunTrackingApp() = default;

  virtual bool Init() = 0;

  virtual void Run() = 0;

  virtual bool Cleanup() = 0;

  virtual bool RequestStop() { return false; }
};

}  // namespace voltiris