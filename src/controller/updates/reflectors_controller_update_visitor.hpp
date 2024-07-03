#pragma once

namespace voltiris::controller {

class CurrentConfigurationUpdate;
class ASTUpdate;
class BusConnectedUpdate;
class BusDisconnectedUpdate;
class BusListUpdate;
class CPUpdate;
class ReflectorStateUpdate;
class ReflectorsSizeUpdate;
class ErrorUpdate;
class StatusUpdate;

class ReflectorsControllerUpdateVisitor {
public:
  virtual ~ReflectorsControllerUpdateVisitor() = default;

  virtual void ProcessUpdate(const CurrentConfigurationUpdate&) = 0;
  virtual void ProcessUpdate(const ASTUpdate&) = 0;
  virtual void ProcessUpdate(const BusConnectedUpdate&) = 0;
  virtual void ProcessUpdate(const BusDisconnectedUpdate&) = 0;
  virtual void ProcessUpdate(const BusListUpdate&) = 0;
  virtual void ProcessUpdate(const CPUpdate&) = 0;
  virtual void ProcessUpdate(const ReflectorStateUpdate&) = 0;
  virtual void ProcessUpdate(const ReflectorsSizeUpdate&) = 0;
  virtual void ProcessUpdate(const ErrorUpdate&) = 0;
  virtual void ProcessUpdate(const StatusUpdate&) = 0;
};

}  // namespace voltiris::controller