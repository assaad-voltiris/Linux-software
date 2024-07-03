#pragma once

#include <sun_tracking_app.hpp>

#include <controller/reflectors_controller.hpp>
#include <presentation/ui/renderer.hpp>
#include <presentation/ui/st_window.hpp>

namespace voltiris {

class StandaloneSunTrackingApp : public SunTrackingApp {
public:
  explicit StandaloneSunTrackingApp();
  ~StandaloneSunTrackingApp() = default;

  bool Init();

  void Run();

  bool Cleanup();

private:
  controller::ReflectorsController _controller;

  presentation::ui::Renderer _renderer;
  presentation::ui::STWindow _window;
};

}  // namespace voltiris