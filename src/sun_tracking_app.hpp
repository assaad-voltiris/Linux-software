#pragma once

#include <controller/reflectors_controller.hpp>
#include <presentation/ui/renderer.hpp>
#include <presentation/ui/st_window.hpp>

namespace voltiris {

class SunTrackingApp {
public:
  explicit SunTrackingApp();
  ~SunTrackingApp() = default;

  bool Init();

  void Run();

  bool Cleanup();

private:
  controller::ReflectorsController _controller;

  presentation::ui::Renderer _renderer;
  presentation::ui::STWindow _window;
};

}  // namespace voltiris