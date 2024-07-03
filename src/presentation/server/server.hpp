#pragma once

#include <memory>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <controller/controller_commands_handler.hpp>
#include <controller/controller_update_listener.hpp>

namespace voltiris::presentation::server {

class Server : public controller::ControllerUpdateListener {
public:
  explicit Server(std::int32_t port, controller::ControllerCommandsHandler& commands_handler);
  ~Server() = default;

  bool Init();
  bool Cleanup();

  bool Run();
  bool RequestStop();


private:
  // ReflectorsDataObserver interface
  void OnUpdate(std::unique_ptr<controller::ReflectorsControllerUpdate> update) override;

private:
  using server = websocketpp::server<websocketpp::config::asio>;

  void OnOpen(server* s, websocketpp::connection_hdl hdl);
  void OnMessage(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg);

  std::int32_t _port = -1;
  controller::ControllerCommandsHandler& _commands_handler;

  server _server;
  server::connection_ptr _connection;
};

}  // namespace voltiris::presentation::server