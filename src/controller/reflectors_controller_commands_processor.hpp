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

class ReflectorsControllerCommandsProcessor : public ReflectorsControllerCommandVisitor {
public:
  static std::vector<ReflectorState> ProcessLoadConfigurationCommand(const LoadConfigurationCommand &command);
  static std::tuple<double, double> ProcessValuesUpdateCommand(const ValuesUpdateCommand &command, double starting_hra, double acceleration_factor);
  static std::int32_t ProcessConnectCommand(const ConnectCommand &command, std::int32_t com_port);
  static std::int32_t ProcessDisconnectCommand(const DisconnectCommand &command, std::int32_t com_port);
  static void ProcessInitializationCommand(const InitializationCommand &command, std::int32_t com_port, std::vector<ReflectorState> &reflectors);
  static void ProcessReadCommand(const ReadCommand &command, std::int32_t com_port, std::vector<ReflectorState> &reflectors);
  static void ProcessFlashCommand(const FlashCommand &command, std::int32_t com_port, std::vector<ReflectorState> &reflectors);
  static void ProcessRebootCommand(const RebootCommand &command, std::int32_t com_port, std::vector<ReflectorState> &reflectors);
  static void ProcessSetPositionCommand(const SetPositionCommand &command, std::int32_t com_port, std::vector<ReflectorState> &reflectors);
  static void ProcessGoCommand(const GoCommand &command, std::int32_t com_port, std::vector<ReflectorState> &reflectors);
  static void ProcessManualMoveCommand(const ManualMoveCommand &command, std::int32_t com_port, std::vector<ReflectorState> &reflectors);
};

}  // namespace voltiris::controller