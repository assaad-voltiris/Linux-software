#include "sun_tracking_app.hpp"

#include <thread>

namespace voltiris {

SunTrackingApp::SunTrackingApp() : _window(_controller) {}

bool SunTrackingApp::Init() {
  _controller.RegisterDataObserver(_window);

  return _renderer.Init();
}

void SunTrackingApp::Run() {
  _controller.Start();

  _renderer.Run(_window);

  _controller.Stop();
}

bool SunTrackingApp::Cleanup() { return _renderer.Cleanup(); }

} // namespace voltiris
