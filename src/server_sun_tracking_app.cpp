#include "server_sun_tracking_app.hpp"

#include <thread>

namespace voltiris {

ServerSunTrackingApp::ServerSunTrackingApp(std::int32_t port) : _controller(), _server(port, _controller) {}

bool ServerSunTrackingApp::Init() {
  _controller.RegisterDataObserver(_server);

  return _server.Init();
}

void ServerSunTrackingApp::Run() {
  _controller.Start();

  _server.Run();

  _controller.Stop();
}

bool ServerSunTrackingApp::Cleanup() { return true; }

bool ServerSunTrackingApp::RequestStop() {
  _server.RequestStop();
  return true;
}

}  // namespace voltiris
