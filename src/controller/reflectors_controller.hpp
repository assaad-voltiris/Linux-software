#pragma once

#include <array>
#include <atomic>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

#include <controller/commands/reflectors_controller_command_visitor.hpp>
#include <controller/controller_commands_handler.hpp>
#include <controller/controller_update_listener.hpp>

namespace voltiris::controller {

class ReflectorsController : public ControllerCommandsHandler, public ReflectorsControllerCommandVisitor {
public:
  explicit ReflectorsController();

  void Start();
  void Stop();

  inline void RegisterDataObserver(ControllerUpdateListener &update_listener) { _update_listener = &update_listener; }
  void ProcessCommand(const RequestConfigurationCommand &command) override;

private:
  void OnCommand(std::unique_ptr<ReflectorsControllerCommand> command) override;

  void ProcessCommand(const LoadConfigurationCommand &command) override;
  void ProcessCommand(const ValuesUpdateCommand &command) override;
  void ProcessCommand(const ConnectCommand &command) override;
  void ProcessCommand(const DisconnectCommand &command) override;
  void ProcessCommand(const InitializationCommand &command) override;
  void ProcessCommand(const ReadCommand &command) override;
  void ProcessCommand(const FlashCommand &command) override;
  void ProcessCommand(const RebootCommand &command) override;
  void ProcessCommand(const SetPositionCommand &command) override;
  void ProcessCommand(const GoCommand &command) override;

private:
  void ControllerThreadExecute();

  void ExecuteCommands();
  void UpdateData();

  ControllerUpdateListener *_update_listener = nullptr;

  std::atomic<bool> _is_working = false;
  std::thread _controller_thread;

  std::queue<std::unique_ptr<ReflectorsControllerCommand>> _commands_queue;
  std::mutex _commands_queue_mutex;

  std::int32_t _com_port = -1;

  double _cycles_frequency = 1;
  bool _hra_enabled = false;
  double _hra = 200;
  double _hra_input = 0;
  double _acceleration_factor = 1;

  double _latitude = 0;
  double _longitude = 0;

  // Internal variables
  std::vector<ReflectorState> _reflectors;
  std::vector<ReflectorState> _reflectors_copy;

  std::chrono::steady_clock::time_point _start_time;

  // Old
  double _temp_hra_old = 200;
  double _hra_old = 200;
  double _hrar_old = 0;
  double _acceleration_factor_old = 0;
};

}  // namespace voltiris::controller