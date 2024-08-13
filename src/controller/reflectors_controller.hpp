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
#include <controller/reflectors_controller_internal_state.hpp>
#include <controller/utils/changeable.hpp>

namespace voltiris::controller {

class ReflectorsController : public ControllerCommandsHandler, public ReflectorsControllerCommandVisitor {
public:
  explicit ReflectorsController();

  void Start();
  void Stop();

  inline void RegisterDataObserver(ControllerUpdateListener &update_listener) { _update_listener = &update_listener; }

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
  void ProcessCommand(const ManualMoveCommand &command) override;
  void ProcessCommand(const StartTrackingCommand &command) override;
  void ProcessCommand(const StopTrackingCommand &command) override;
  void ProcessCommand(const AutomaticMoveCommand &command) override;
  void ProcessCommand(const RequestConfigurationCommand &command) override;
  void ProcessCommand(const StopMovementCommand &command) override;

private:
  void ControllerThreadExecute();

  void ExecuteCommands();
  ReflectorsControllerIterationState UpdateData(double old_hra);
  void ProcessUpdates(const ReflectorsControllerIterationState &new_state);
  void ProcessTracking(const ReflectorsControllerIterationState &current_state);
  void ProcessSingleMovement();
  void ProcessSingleCalibrationMovement(ReflectorState &reflector);

  ControllerUpdateListener *_update_listener = nullptr;

  std::atomic<bool> _is_working = false;
  std::thread _controller_thread;

  std::queue<std::unique_ptr<ReflectorsControllerCommand>> _commands_queue;
  std::mutex _commands_queue_mutex;

  ControllerStatus _status = ControllerStatus::kOperating;
  ControllerStatus _prev_status = ControllerStatus::kOperating;

  std::int32_t _com_port = -1;

  ReflectorsControllerInternalState _internal_state;
  ReflectorsControllerIterationState _iteration_state;

  // Internal variables
  std::vector<ReflectorState> _reflectors;
  std::vector<ReflectorState> _reflectors_copy;
};

}  // namespace voltiris::controller