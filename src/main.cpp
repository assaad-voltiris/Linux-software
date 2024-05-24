#include <spdlog/spdlog.h>

#include <sun_tracking_app.hpp>

int main(int, char **) {
  spdlog::default_logger()->set_level(spdlog::level::debug);

  voltiris::SunTrackingApp app;

  if (app.Init()) {
    app.Run();
    app.Cleanup();
  }

  return 0;
}
