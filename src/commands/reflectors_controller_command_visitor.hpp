#pragma once

namespace voltiris {

class LoadConfigurationCommand;
class ValuesUpdateCommand;

class ReflectorsControllerCommandVisitor {
public:
  virtual void ProcessCommand(const LoadConfigurationCommand&) = 0;
  virtual void ProcessCommand(const ValuesUpdateCommand&) = 0;
};

}