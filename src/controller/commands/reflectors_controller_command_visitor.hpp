#pragma once

namespace voltiris::controller {

class LoadConfigurationCommand;
class ValuesUpdateCommand;
class ConnectCommand;
class DisconnectCommand;
class InitializationCommand;
class ReadCommand;

class ReflectorsControllerCommandVisitor {
public:
  virtual void ProcessCommand(const ConnectCommand&) = 0;
  virtual void ProcessCommand(const DisconnectCommand&) = 0;

  virtual void ProcessCommand(const LoadConfigurationCommand&) = 0;
  virtual void ProcessCommand(const ValuesUpdateCommand&) = 0;

  virtual void ProcessCommand(const InitializationCommand&) = 0;
  virtual void ProcessCommand(const ReadCommand&) = 0;
};

}  // namespace voltiris::controller