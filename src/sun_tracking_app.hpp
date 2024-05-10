#pragma once

#include <reflectors_controller.hpp>
#include <renderer.hpp>
#include <st_window.hpp>

namespace voltiris {

class SunTrackingApp {
public:
  explicit SunTrackingApp();
  ~SunTrackingApp() = default;

  bool Init();

  void Run();

  bool Cleanup();

private:
  ReflectorsController _controller;

  Renderer _renderer;
  STWindow _window;
};

}