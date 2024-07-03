#include "client_sun_tracking_app.hpp"

#include <thread>

namespace voltiris {

ClientSunTrackingApp::ClientSunTrackingApp(const std::string& address, std::int32_t port) : _client(address, port), _window(_client) {}

bool ClientSunTrackingApp::Init() {
  if (!_client.Init()) { return false; }

  _client.RegisterDataObserver(_window);

  return _renderer.Init();
}

void ClientSunTrackingApp::Run() {
  _client.Start();

  _window.Init();

  _renderer.Run(_window);

  _client.Stop();
}

bool ClientSunTrackingApp::Cleanup() {
  _client.Cleanup();
  return true;
}

}  // namespace voltiris
