#pragma once

#include <sun_tracking_app.hpp>

#include <controller/reflectors_controller.hpp>
#include <presentation/client/client.hpp>
#include <presentation/ui/renderer.hpp>
#include <presentation/ui/st_window.hpp>

namespace voltiris {

class ClientSunTrackingApp : public SunTrackingApp {
public:
  explicit ClientSunTrackingApp(const std::string& address, std::int32_t port);
  ~ClientSunTrackingApp() override = default;

  bool Init() override;

  void Run() override;

  bool Cleanup() override;

private:
  presentation::client::Client _client;

  presentation::ui::Renderer _renderer;
  presentation::ui::STWindow _window;
};

}  // namespace voltiris