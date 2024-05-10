#include <sun_tracking_app.hpp>

int main(int, char **) {
  voltiris::SunTrackingApp app;

  if (app.Init()) {
    app.Run();
    app.Cleanup();
  }

  return 0;
}
