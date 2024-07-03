#include "server.hpp"

#include <spdlog/spdlog.h>

#include <controller/commands/reflectors_controller_command.hpp>

namespace voltiris::presentation::server {

namespace {}  // namespace

Server::Server(std::int32_t port, controller::ControllerCommandsHandler& commands_handler) : _port(port), _commands_handler(commands_handler) {}

bool Server::Init() {
  using namespace websocketpp::lib::placeholders;

  try {
    // Set logging settings
    _server.set_access_channels(websocketpp::log::alevel::all);
    _server.clear_access_channels(websocketpp::log::alevel::frame_payload);

    // Initialize Asio
    _server.init_asio();

    // Register our message handler
    _server.set_message_handler(std::bind(&Server::OnMessage, this, &_server, _1, _2));
    _server.set_open_handler(std::bind(&Server::OnOpen, this, &_server, _1));

    _server.listen(_port);

    // Start the server accept loop
    _server.start_accept();
    return true;
  } catch (websocketpp::exception const& e) {
    spdlog::error("Server init websocketpp exception: \"{}\".", e.what());
    return false;
  } catch (...) {
    spdlog::error("Server init exception.");
    return false;
  }
}

bool Server::Cleanup() { return false; }

bool Server::Run() {
  try {
    _server.run();
    return true;
  } catch (websocketpp::exception const& e) {
    spdlog::error("Server run websocketpp exception: \"{}\".", e.what());
    return false;
  } catch (...) {
    spdlog::error("Server run exception.");
    return false;
  }
}

bool Server::RequestStop() {
  if (_connection) { _connection->close(websocketpp::close::status::normal, ""); }
  _server.stop();
  return true;
}

void Server::OnMessage(Server::server* s, websocketpp::connection_hdl hdl, server::message_ptr msg) {
  spdlog::info("Message received: {}", msg->get_payload());
  _commands_handler.OnCommand(controller::ReflectorsControllerCommand::FromJson(msg->get_payload()));
}

void Server::OnUpdate(std::unique_ptr<controller::ReflectorsControllerUpdate> update) {
  if (_connection) {
    auto error = _connection->send(update->ToJson(), websocketpp::frame::opcode::TEXT);
    if (error) {
      spdlog::error("Server message sending error: {}", error.message());
      if (_connection->get_state() == websocketpp::session::state::closed) { _connection.reset(); }
    }
  }
}

void Server::OnOpen(Server::server* s, websocketpp::connection_hdl hdl) {
  spdlog::info("New connection received.");
  _connection = s->get_con_from_hdl(hdl);
}

}  // namespace voltiris::presentation::server
