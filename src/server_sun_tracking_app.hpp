#pragma once

#include <sun_tracking_app.hpp>

#include <controller/reflectors_controller.hpp>
#include <presentation/server/server.hpp>
#include <presentation/ui/renderer.hpp>
#include <presentation/ui/st_window.hpp>

namespace voltiris {

class ServerSunTrackingApp : public SunTrackingApp {
public:
  explicit ServerSunTrackingApp(std::int32_t port);
  ~ServerSunTrackingApp() override = default;

  bool Init() override;

  void Run() override;

  bool Cleanup() override;

  bool RequestStop() override;

private:
  voltiris::controller::ReflectorsController _controller;

  voltiris::presentation::server::Server _server;
};

}  // namespace voltiris