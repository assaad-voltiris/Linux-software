#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <array>

#include <commands/reflectors_controller_command_visitor.hpp>
#include <commands/values_update_command.hpp>
#include <reflectors_commands_observer.hpp>
#include <reflectors_data_observer.hpp>
#include <resources.hpp>
#include <rs485.hpp>

namespace voltiris {

class ReflectorsController : public ReflectorsCommandsObserver,
                             public ReflectorsControllerCommandVisitor {
public:
  explicit ReflectorsController();

  void Start();
  void Stop();

  inline void RegisterDataObserver(ReflectorsDataObserver &data_observer) {
    _data_observer = &data_observer;
  }

private:
  void OnCommand(std::unique_ptr<ReflectorsControllerCommand> command) override;

  void ProcessCommand(const LoadConfigurationCommand &command) override;
  void ProcessCommand(const ValuesUpdateCommand &command) override;

private:
  void ControllerThreadExecute();

  void ExecuteCommands();
  void UpdateData();

  ReflectorsDataObserver *_data_observer;

  std::atomic<bool> _is_working;
  std::thread _controller_thread;

  std::queue<std::unique_ptr<ReflectorsControllerCommand>> _commands_queue;
  std::mutex _commands_queue_mutex;

  double _cycles_frequency = 0;
  bool _hra_enabled = false;
  double _hra = 0;
  double _hra_input = 0;
  bool _acceleration_factor_empty = true;
  double _acceleration_factor = 0;

  double _latitude = 0;
  double _longitude = 0;

  // Internal variables
  std::array<RS485, 6> _rs_adapters;

  std::chrono::steady_clock::time_point _start_time;

  // Old
  double _temp_hra_old = 200;
  double _hra_old = 200;
  double _hrar_old = 0;
  double _acceleration_factor_old = 0;
};

} // namespace voltiris