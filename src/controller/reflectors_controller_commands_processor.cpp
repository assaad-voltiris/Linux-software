#include "reflectors_controller_commands_processor.hpp"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>

#include <spdlog/spdlog.h>

#include <controller/updates/ast_update.hpp>
#include <controller/updates/bus_connected_update.hpp>
#include <controller/updates/bus_disconnected_update.hpp>
#include <controller/updates/bus_list_update.hpp>
#include <controller/updates/cp_update.hpp>
#include <controller/updates/current_configuration_update.hpp>
#include <controller/updates/error_update.hpp>
#include <controller/updates/reflector_state_update.hpp>
#include <controller/updates/reflectors_size_update.hpp>
#include <controller/updates/status_update.hpp>

#include <controller/commands/connect_command.hpp>
#include <controller/commands/disconnect_command.hpp>
#include <controller/commands/flash_command.hpp>
#include <controller/commands/go_command.hpp>
#include <controller/commands/initialization_command.hpp>
#include <controller/commands/load_configuration_command.hpp>
#include <controller/commands/manual_move_command.hpp>
#include <controller/commands/read_command.hpp>
#include <controller/commands/reboot_command.hpp>
#include <controller/commands/set_position_command.hpp>
#include <controller/commands/values_update_command.hpp>

#include <controller/utils/communication.hpp>
#include <controller/utils/converters.hpp>
#include <controller/utils/reflector.hpp>
#include <controller/utils/sun_math.hpp>
#include <controller/utils/time.hpp>

namespace voltiris::controller {

namespace {

static const double kMaxSecu = 39.0;

}  // namespace

std::vector<ReflectorState> ReflectorsControllerCommandsProcessor::ProcessLoadConfigurationCommand(const LoadConfigurationCommand &command) {
  spdlog::debug("LoadConfigurationCommand received.");

  std::vector<ReflectorState> reflectors;

  if (command.GetFilePath().empty()) {
    reflectors = utils::LoadReflectorsFromConfiguration(command.GetFileContent());
  } else {
    reflectors = utils::LoadReflectorsFromConfigurationFile(command.GetFilePath());
  }

  return reflectors;
}

std::tuple<double, double> ReflectorsControllerCommandsProcessor::ProcessValuesUpdateCommand(const ValuesUpdateCommand &command, double starting_hra,
                                                                                             double acceleration_factor) {
  double starting_hra_new = command.GetStartingHra().has_value() ? command.GetStartingHra().has_value() : starting_hra;
  double acceleration_factor_new = command.GetAccelerationFactor().has_value() ? command.GetAccelerationFactor().has_value() : acceleration_factor;
  return {starting_hra_new, acceleration_factor_new};
}

std::int32_t ReflectorsControllerCommandsProcessor::ProcessConnectCommand(const ConnectCommand &command, std::int32_t com_port) {
  if (com_port != -1) { throw std::runtime_error("Port already connected."); }

  return utils::Connect(command.GetBusName());
}

std::int32_t ReflectorsControllerCommandsProcessor::ProcessDisconnectCommand(const DisconnectCommand &command, std::int32_t com_port) {
  utils::Disconnect(com_port);
  return -1;
}

void ReflectorsControllerCommandsProcessor::ProcessInitializationCommand(const InitializationCommand &command, std::int32_t com_port,
                                                                         std::vector<ReflectorState> &reflectors) {
  if (com_port == -1) { throw std::runtime_error("Reflectors not connected."); }

  bool result = true;
  for (auto &reflector : reflectors) {
    result &= utils::WakeUp(com_port, reflector);
    result &= utils::Initialize(com_port, reflector);
    result &= utils::ReadPositioningData(com_port, reflector);
    result &= utils::SetHall(com_port, reflector);
  }

  if (!result) { throw std::runtime_error("Reflectors initialization error."); }
}

void ReflectorsControllerCommandsProcessor::ProcessReadCommand(const ReadCommand &command, std::int32_t com_port, std::vector<ReflectorState> &reflectors) {
  if (com_port == -1) { throw std::runtime_error("Reflectors not connected."); }

  bool result = true;
  for (auto &reflector : reflectors) {
    result &= utils::WakeUp(com_port, reflector);
    result &= utils::ReadPositioningData(com_port, reflector);
  }

  if (!result) { throw std::runtime_error("Reflectors initialization error."); }
}

void ReflectorsControllerCommandsProcessor::ProcessFlashCommand(const FlashCommand &command, std::int32_t com_port, std::vector<ReflectorState> &reflectors) {
  if (com_port == -1) { throw std::runtime_error("Reflectors not connected."); }

  bool result = true;
  for (auto &reflector : reflectors) {
    result &= utils::WakeUp(com_port, reflector);
    result &= utils::Flash(com_port, reflector);
  }

  if (!result) { throw std::runtime_error("Reflectors initialization error."); }
}

void ReflectorsControllerCommandsProcessor::ProcessRebootCommand(const RebootCommand &command, std::int32_t com_port, std::vector<ReflectorState> &reflectors) {
  if (com_port == -1) { throw std::runtime_error("Reflectors not connected."); }

  bool result = true;
  for (auto &reflector : reflectors) {
    result &= utils::WakeUp(com_port, reflector);
    result &= utils::Reboot(com_port, reflector);
    result &= utils::ReadPositioningData(com_port, reflector);
  }

  if (!result) { throw std::runtime_error("Reflectors initialization error."); }
}

void ReflectorsControllerCommandsProcessor::ProcessSetPositionCommand(const SetPositionCommand &command, std::int32_t com_port,
                                                                      std::vector<ReflectorState> &reflectors) {
  if (com_port == -1) { throw std::runtime_error("Reflectors not connected."); }

  bool result = true;

  result &= utils::WakeUp(com_port, reflectors[command.GetReflectorIndex()]);
  result &= utils::SetPosition(com_port, reflectors[command.GetReflectorIndex()], command.GetAzimuth(), command.GetElevation());
  result &= utils::ReadPositioningData(com_port, reflectors[command.GetReflectorIndex()]);

  if (!result) { throw std::runtime_error("Reflectors initialization error."); }
}

void ReflectorsControllerCommandsProcessor::ProcessGoCommand(const GoCommand &command, std::int32_t com_port, std::vector<ReflectorState> &reflectors) {
  if (com_port == -1) { throw std::runtime_error("Reflectors not connected."); }

  reflectors[command.GetReflectorIndex()].should_be_moved_azimuth = command.GetAzimuth();
  reflectors[command.GetReflectorIndex()].should_be_moved_elevation = command.GetElevation();
}

void ReflectorsControllerCommandsProcessor::ProcessManualMoveCommand(const ManualMoveCommand &command, std::int32_t com_port,
                                                                     std::vector<ReflectorState> &reflectors) {
  using Direction = ManualMoveCommand::Direction;

  if (com_port == -1) { throw std::runtime_error("Reflectors not connected."); }

  double n_step_mvmt = std::ceil(std::abs(command.GetStep()) / 10.0);
  double azimuth = command.GetDirection() == Direction::kUp || command.GetDirection() == Direction::kDown ? 0 : command.GetStep() / n_step_mvmt;
  double elevation = command.GetDirection() == Direction::kWest || command.GetDirection() == Direction::kEast ? 0 : command.GetStep() / n_step_mvmt;
  if (command.GetDirection() == ManualMoveCommand::Direction::kDown) { elevation = -elevation; }
  if (command.GetDirection() == ManualMoveCommand::Direction::kEast) { azimuth = -azimuth; }

  double n_step_azimuth_f = std::abs(azimuth / 10.0);
  double n_step_elevation_f = std::abs(elevation / 10.0);
  double n_step_f = std::max(n_step_azimuth_f, n_step_elevation_f);
  auto n_step = static_cast<std::size_t>(std::ceil(n_step_f));

  for (std::size_t reflector_index : command.GetReflectors()) {
    auto &reflector = reflectors[reflector_index];
    double delta_azimuth_mm = -(azimuth / reflector.a1);
    double delta_elevation_mm = -((elevation / reflector.a3) - reflector.a2 * (azimuth / static_cast<double>(n_step)) / (reflector.a1 * reflector.a3));

    double target_pos_azimuth_mm = delta_azimuth_mm + reflector.actual_position_azimuth_mm;
    double target_pos_elevation_mm = delta_elevation_mm + reflector.actual_position_elevation_mm;

    reflector.should_be_moved_azimuth = target_pos_azimuth_mm;
    reflector.should_be_moved_elevation = target_pos_elevation_mm;

    //    for (std::size_t j = 0; j < n_step; j++) {
    //      auto &reflector = reflectors[reflector_index];
    //
    //      double delta_azimuth_mm = -((azimuth / static_cast<double>(n_step)) / reflector.a1);
    //      double delta_elevation_mm = -(((elevation / static_cast<double>(n_step)) / reflector.a3) -
    //                                    reflector.a2 * (azimuth / static_cast<double>(n_step)) / (reflector.a1 * reflector.a3));
    //
    //      double target_pos_azimuth_mm = delta_azimuth_mm + reflector.actual_position_azimuth_mm;
    //      double target_pos_elevation_mm = delta_elevation_mm + reflector.actual_position_elevation_mm;
    //
    //    }
  }
}

void ReflectorsControllerCommandsProcessor::ProcessAutomaticMoveCommand(const AutomaticMoveCommand &command, std::vector<ReflectorState> &reflectors) {
  for (auto &reflector : reflectors) {
    if (command.GetCalibrate()) {
      reflector.calibration_azimuth_cycles = 0;
      reflector.calibration_elevation_cycles = 0;
      reflector.calibration_doubleclicked_azimuth = false;
      reflector.calibration_doubleclicked_elevation = false;
    }

    if (command.GetTheoreticalPosition()) {
      reflector.should_be_moved_azimuth = reflector.theoretical_position_azimuth_mm;
      reflector.should_be_moved_elevation = reflector.theoretical_position_elevation_mm;
    }

    if (command.GetInitPosition()) {
      reflector.should_be_moved_azimuth = reflector.calibration_azimuth_mm;
      reflector.should_be_moved_elevation = reflector.calibration_elevation_mm;
    }

    if (command.GetPerpToSun()) {
      double offset = utils::ConvertAzimuth_Mm2Deg(reflector, reflector.theoretical_position_azimuth_mm) < 180 ? 90. : -90.;
      double az = utils::ConvertAzimuth_Deg2Mm(reflector, utils::ConvertAzimuth_Mm2Deg(reflector, reflector.theoretical_position_azimuth_mm) + offset);

      reflector.should_be_moved_azimuth = az;
      reflector.should_be_moved_elevation = reflector.theoretical_position_elevation_mm;
    }
  }
}

}  // namespace voltiris::controller
