#include "standalone_sun_tracking_app.hpp"

#include <thread>

namespace voltiris {

StandaloneSunTrackingApp::StandaloneSunTrackingApp() : _window(_controller, false) {}

bool StandaloneSunTrackingApp::Init() {
  _controller.RegisterDataObserver(_window);

  _window.Init();

  return _renderer.Init();
}

void StandaloneSunTrackingApp::Run() {
  _controller.Start();

  _renderer.Run(_window);

  _controller.Stop();
}

bool StandaloneSunTrackingApp::Cleanup() { return _renderer.Cleanup(); }

}  // namespace voltiris
