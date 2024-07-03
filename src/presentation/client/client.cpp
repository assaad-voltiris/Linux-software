#include "client.hpp"

#include <spdlog/spdlog.h>

namespace voltiris::presentation::client {

namespace {}  // namespace

Client::Client(const std::string& address, std::int32_t port) : _address(address), _port(port) {}

bool Client::Init() {
  using namespace websocketpp::lib::placeholders;

  std::string uri = "ws://" + _address + ":" + std::to_string(_port);

  try {
    // Set logging to be pretty verbose (everything except message payloads)
    _client.set_access_channels(websocketpp::log::alevel::all);
    _client.clear_access_channels(websocketpp::log::alevel::frame_payload);

    // Initialize ASIO
    _client.init_asio();

    // Register our message handler
    _client.set_message_handler(bind(&Client::OnMessage, this, &_client, _1, _2));

    websocketpp::lib::error_code ec;
    _connection = _client.get_connection(uri, ec);
    if (ec) {
      spdlog::error("Could not create connection because: {}", ec.message());
      return false;
    }

    // Note that connect here only requests a connection. No network messages are
    // exchanged until the event loop starts running in the next line.
    _client.connect(_connection);

  } catch (websocketpp::exception const& e) {
    spdlog::error("Client init websocketpp exception: \"{}\".", e.what());
    return false;
  } catch (...) {
    spdlog::error("Client init exception.");
    return false;
  }
  return true;
}

bool Client::Cleanup() { return false; }

void Client::Start() {
  _thread = std::thread([this]() { _client.run(); });
}

void Client::Stop() {
  if (_connection) { _connection->close(websocketpp::close::status::normal, ""); }
  _client.stop();
}

void Client::OnCommand(std::unique_ptr<controller::ReflectorsControllerCommand> command) {
  if (_connection) {
    auto error = _connection->send(command->ToJson(), websocketpp::frame::opcode::TEXT);
    if (error) { spdlog::error("Client message sending error: {}", error.message()); }
  }
}

void Client::OnMessage(Client::client* s, websocketpp::connection_hdl hdl, client::message_ptr msg) {
  spdlog::info("Message received: {}", msg->get_payload());
  _update_listener->OnUpdate(controller::ReflectorsControllerUpdate::FromJson(msg->get_payload()));
}

}  // namespace voltiris::presentation::client
