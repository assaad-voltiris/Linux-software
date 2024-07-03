#include <spdlog/spdlog.h>

#include <client_sun_tracking_app.hpp>
#include <server_sun_tracking_app.hpp>
#include <standalone_sun_tracking_app.hpp>
#include <sun_tracking_app.hpp>

namespace {
const std::string kRunAsServerParameter1 = "-s";
const std::string kRunAsServerParameter2 = "--server";

const std::string kRunAsClientParameter1 = "-c";
const std::string kRunAsClientParameter2 = "--client";

const std::string kAddressParameter1 = "-a";
const std::string kAddressParameter2 = "--address";

const std::string kPortParameter1 = "-p";
const std::string kPortParameter2 = "--port";
}  // namespace

std::unique_ptr<voltiris::SunTrackingApp> app;

void SignalHandler(int) { app->RequestStop(); }

int main(int args_count, char** args) {
  spdlog::default_logger()->set_level(spdlog::level::debug);

  bool run_as_server = false;
  bool run_as_client = false;

  std::string connection_address;
  std::int32_t connection_port = -1;

  for (std::int32_t i = 0; i < args_count; ++i) {
    if (kRunAsServerParameter1 == args[i] || kRunAsServerParameter2 == args[i]) { run_as_server = true; }
    if (kRunAsClientParameter1 == args[i] || kRunAsClientParameter2 == args[i]) { run_as_client = true; }
    if ((kAddressParameter1 == args[i] || kAddressParameter2 == args[i]) && i + 1 != args_count) { connection_address = args[i + 1]; }
    if ((kPortParameter1 == args[i] || kPortParameter2 == args[i]) && i + 1 != args_count) { connection_port = std::stoi(std::string(args[i + 1])); }
  }

  if (run_as_server && connection_port == -1) {
    spdlog::error(R"(In "server" mode port should be specified. Please add "-p 2345".)");
    return 1;
  }

  if (run_as_client && connection_address.empty() && connection_port == -1) {
    spdlog::error(R"(In "client" mode address and port should be specified. Please add "-a 192.168.0.1 -p 2345".)");
    return 1;
  }

  if (run_as_client) {
    app = std::make_unique<voltiris::ClientSunTrackingApp>(connection_address, connection_port);
    spdlog::info("VoltirisST started in client mode.");
  }
#if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32)
  else if (run_as_server) {
    std::signal(SIGINT, &SignalHandler);
    std::signal(SIGTERM, &SignalHandler);
    std::signal(SIGKILL, &SignalHandler);

    app = std::make_unique<voltiris::ServerSunTrackingApp>(connection_port);
    spdlog::info("VoltirisST started in server mode.");
  } else {
    app = std::make_unique<voltiris::StandaloneSunTrackingApp>();
    spdlog::info("VoltirisST started in standalone mode.");
  }
#endif

  if (app->Init()) {
    app->Run();
    app->Cleanup();
  }

  return 0;
}
