#pragma once

namespace voltiris::controller {

class LoadConfigurationCommand;
class ValuesUpdateCommand;
class ConnectCommand;
class DisconnectCommand;
class InitializationCommand;
class ReadCommand;
class FlashCommand;
class RebootCommand;
class SetPositionCommand;
class GoCommand;
class RequestConfigurationCommand;
class ManualMoveCommand;
class StartTrackingCommand;
class StopTrackingCommand;
class AutomaticMoveCommand;
class StopMovementCommand;

class ReflectorsControllerCommandVisitor {
public:
  virtual ~ReflectorsControllerCommandVisitor() = default;

  virtual void ProcessCommand(const ConnectCommand&) = 0;
  virtual void ProcessCommand(const DisconnectCommand&) = 0;

  virtual void ProcessCommand(const LoadConfigurationCommand&) = 0;
  virtual void ProcessCommand(const ValuesUpdateCommand&) = 0;

  virtual void ProcessCommand(const InitializationCommand&) = 0;
  virtual void ProcessCommand(const ReadCommand&) = 0;
  virtual void ProcessCommand(const FlashCommand&) = 0;
  virtual void ProcessCommand(const RebootCommand&) = 0;
  virtual void ProcessCommand(const SetPositionCommand&) = 0;
  virtual void ProcessCommand(const GoCommand&) = 0;

  virtual void ProcessCommand(const RequestConfigurationCommand&) = 0;

  virtual void ProcessCommand(const ManualMoveCommand&) = 0;

  virtual void ProcessCommand(const StartTrackingCommand&) = 0;
  virtual void ProcessCommand(const StopTrackingCommand&) = 0;

  virtual void ProcessCommand(const AutomaticMoveCommand&) = 0;
  virtual void ProcessCommand(const StopMovementCommand&) = 0;
};

}  // namespace voltiris::controller