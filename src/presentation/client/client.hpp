#pragma once

#include <memory>
#include <vector>

#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>

#include <controller/controller_commands_handler.hpp>
#include <controller/controller_update_listener.hpp>

namespace voltiris::presentation::client {

class Client : public controller::ControllerCommandsHandler {
public:
  explicit Client(const std::string& address, std::int32_t port);
  ~Client() = default;

  bool Init();
  bool Cleanup();

  void Start();
  void Stop();

  inline void RegisterDataObserver(controller::ControllerUpdateListener& update_listener) { _update_listener = &update_listener; }

private:
  // ControllerCommandsHandler interface
  void OnCommand(std::unique_ptr<controller::ReflectorsControllerCommand> command) override;

private:
  using client = websocketpp::client<websocketpp::config::asio_client>;

  void OnMessage(client* s, websocketpp::connection_hdl hdl, client::message_ptr msg);

  std::thread _thread;

  std::string _address;
  std::int32_t _port = -1;
  controller::ControllerUpdateListener* _update_listener = nullptr;

  client _client;
  client::connection_ptr _connection;
};

}  // namespace voltiris::presentation::client