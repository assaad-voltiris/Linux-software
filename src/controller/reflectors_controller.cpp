#include "reflectors_controller.hpp"

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

#include <controller/reflectors_controller_commands_processor.hpp>

namespace voltiris::controller {

namespace {

const int kNightSize = 24;
const double kHRAReturnDefault[kNightSize] = {90.0, 82.5, 75,    67.5,  60.0,  52.5,  45.0,  37.5,  30.0,  22.5,  15.0,  7.5,
                                              1.0,  -7.5, -15.0, -22.5, -30.0, -37.5, -45.0, -52.5, -60.0, -67.5, -75.0, -82.5};

double kAzimuthMax = 80;
double kElevationMin = 16.00;

}  // namespace

ReflectorsController::ReflectorsController() {}

void ReflectorsController::Start() {
  _is_working = true;

  _update_listener->OnUpdate(std::make_unique<BusListUpdate>(utils::GetAvailablePorts()));

  _controller_thread = std::thread(&ReflectorsController::ControllerThreadExecute, this);
}

void ReflectorsController::Stop() {
  if (_com_port != -1) {
    utils::Disconnect(_com_port);
    _update_listener->OnUpdate(std::make_unique<BusDisconnectedUpdate>());
  }

  _is_working = false;
  _controller_thread.join();
}

void ReflectorsController::OnCommand(std::unique_ptr<ReflectorsControllerCommand> command) {
  spdlog::info("New command received.");
  std::lock_guard<std::mutex> lock(_commands_queue_mutex);
  _commands_queue.push(std::move(command));
}

void ReflectorsController::ExecuteCommands() {
  std::unique_ptr<ReflectorsControllerCommand> command;

  std::unique_lock<std::mutex> lock(_commands_queue_mutex);
  while (!_commands_queue.empty()) {
    command = std::move(_commands_queue.front());
    _commands_queue.pop();

    lock.unlock();

    command->Visit(*this);

    lock.lock();
  }
}

void ReflectorsController::ProcessCommand(const LoadConfigurationCommand &command) {
  spdlog::debug("LoadConfigurationCommand received.");
  _reflectors = ReflectorsControllerCommandsProcessor::ProcessLoadConfigurationCommand(command);
  if (!_reflectors.empty()) {
    _internal_state.latitude = _reflectors[0].latitude;
    _internal_state.longitude = _reflectors[0].longitude;
  }
  _update_listener->OnUpdate(std::make_unique<CurrentConfigurationUpdate>(_internal_state.latitude, _internal_state.longitude, _internal_state.cycles_frequency,
                                                                          _internal_state.hra_enabled, _internal_state.hra,
                                                                          _internal_state.acceleration_factor));
}

void ReflectorsController::ProcessCommand(const ValuesUpdateCommand &command) {
  spdlog::debug("ValuesUpdateCommand received.");
  std::tie(_internal_state.hra, _internal_state.acceleration_factor) =
      ReflectorsControllerCommandsProcessor::ProcessValuesUpdateCommand(command, _internal_state.hra, _internal_state.acceleration_factor);
}

void ReflectorsController::ProcessCommand(const ConnectCommand &command) {
  spdlog::debug("ConnectCommand received.");
  _com_port = ReflectorsControllerCommandsProcessor::ProcessConnectCommand(command, _com_port);
  if (_com_port != -1) { _update_listener->OnUpdate(std::make_unique<BusConnectedUpdate>()); }
}

void ReflectorsController::ProcessCommand(const DisconnectCommand &command) {
  spdlog::debug("DisconnectCommand received.");
  _com_port = ReflectorsControllerCommandsProcessor::ProcessDisconnectCommand(command, _com_port);
  _update_listener->OnUpdate(std::make_unique<BusDisconnectedUpdate>());
}

void ReflectorsController::ProcessCommand(const InitializationCommand &command) {
  spdlog::debug("InitializationCommand received.");
  _update_listener->OnUpdate(std::make_unique<StatusUpdate>(ControllerStatus::kCommunicating));
  ReflectorsControllerCommandsProcessor::ProcessInitializationCommand(command, _com_port, _reflectors);
}

void ReflectorsController::ProcessCommand(const ReadCommand &command) {
  spdlog::debug("ReadCommand received.");
  _update_listener->OnUpdate(std::make_unique<StatusUpdate>(ControllerStatus::kCommunicating));
  ReflectorsControllerCommandsProcessor::ProcessReadCommand(command, _com_port, _reflectors);
}

void ReflectorsController::ProcessCommand(const FlashCommand &command) {
  spdlog::debug("FlashCommand received.");
  _update_listener->OnUpdate(std::make_unique<StatusUpdate>(ControllerStatus::kCommunicating));
  ReflectorsControllerCommandsProcessor::ProcessFlashCommand(command, _com_port, _reflectors);
}

void ReflectorsController::ProcessCommand(const RebootCommand &command) {
  spdlog::debug("RebootCommand received.");
  _update_listener->OnUpdate(std::make_unique<StatusUpdate>(ControllerStatus::kCommunicating));
  ReflectorsControllerCommandsProcessor::ProcessRebootCommand(command, _com_port, _reflectors);
}

void ReflectorsController::ProcessCommand(const SetPositionCommand &command) {
  spdlog::debug("SetPositionCommand received.");
  _update_listener->OnUpdate(std::make_unique<StatusUpdate>(ControllerStatus::kCommunicating));
  ReflectorsControllerCommandsProcessor::ProcessSetPositionCommand(command, _com_port, _reflectors);
}

void ReflectorsController::ProcessCommand(const GoCommand &command) {
  spdlog::debug("GoCommand received.");
  _update_listener->OnUpdate(std::make_unique<StatusUpdate>(ControllerStatus::kMoving));
  ReflectorsControllerCommandsProcessor::ProcessGoCommand(command, _com_port, _reflectors);
}

void ReflectorsController::ProcessCommand(const ManualMoveCommand &command) {
  spdlog::debug("MoveCommand received.");
  _update_listener->OnUpdate(std::make_unique<StatusUpdate>(ControllerStatus::kMoving));
  ReflectorsControllerCommandsProcessor::ProcessManualMoveCommand(command, _com_port, _reflectors);
}

void ReflectorsController::ProcessCommand(const RequestConfigurationCommand &command) {
  spdlog::debug("RequestConfigurationCommand received.");
  _update_listener->OnUpdate(std::make_unique<CurrentConfigurationUpdate>(_internal_state.latitude, _internal_state.longitude, _internal_state.cycles_frequency,
                                                                          _internal_state.hra_enabled, _internal_state.hra,
                                                                          _internal_state.acceleration_factor));
  _update_listener->OnUpdate(std::make_unique<BusListUpdate>(utils::GetAvailablePorts()));

  _update_listener->OnUpdate(std::make_unique<ReflectorsSizeUpdate>(_reflectors.size()));
  for (std::size_t i = 0; i < _reflectors.size(); ++i) { _update_listener->OnUpdate(std::make_unique<ReflectorStateUpdate>(i, _reflectors[i])); }

  _update_listener->OnUpdate(_com_port == -1 ? std::unique_ptr<ReflectorsControllerUpdate>(new BusDisconnectedUpdate())
                                             : std::unique_ptr<ReflectorsControllerUpdate>(new BusConnectedUpdate()));
}

void ReflectorsController::ProcessUpdates(const ReflectorsControllerIterationState &new_state) {
  if (_reflectors.size() != _reflectors_copy.size()) {
    _reflectors_copy.resize(_reflectors.size());
    _update_listener->OnUpdate(std::make_unique<ReflectorsSizeUpdate>(_reflectors.size()));
    for (std::size_t i = 0; i < _reflectors.size(); ++i) {
      _reflectors_copy[i] = _reflectors[i];
      _update_listener->OnUpdate(std::make_unique<ReflectorStateUpdate>(i, _reflectors[i]));
    }
  } else {
    for (std::size_t i = 0; i < _reflectors.size(); ++i) {
      if (_reflectors_copy[i] != _reflectors[i]) {
        _reflectors_copy[i] = _reflectors[i];
        _update_listener->OnUpdate(std::make_unique<ReflectorStateUpdate>(i, _reflectors[i]));
      }
    }
  }

  if (_iteration_state != new_state) {
    _iteration_state = new_state;

    _update_listener->OnUpdate(std::make_unique<ASTUpdate>(_iteration_state.latitude, _iteration_state.longitude, _iteration_state.local_hours,
                                                           _iteration_state.local_minutes, _iteration_state.system_hours, _iteration_state.system_minutes,
                                                           _iteration_state.sun_azimuth, _iteration_state.sun_elevation, _iteration_state.sun_ref_azimuth,
                                                           _iteration_state.sun_ref_elevation, _iteration_state.hra, _iteration_state.hrar));
  }
}

ReflectorsControllerIterationState ReflectorsController::UpdateData(double old_hra) {
  ReflectorsControllerIterationState new_state;

  time_t time_t = utils::GetCurrentTimePoint();
  tm utc_time = utils::GetUTCTime(time_t);
  tm local_time = utils::GetLocalTime(time_t);

  double B = utils::GetB(utc_time.tm_mon, utc_time.tm_mday);
  double delta_rad = utils::GetDeltaRad(B);
  double solar_time_dec = utils::GetSolarTimeDec(utc_time.tm_mon, utc_time.tm_mday, _internal_state.longitude, delta_rad, B);
  double fhra = utils::GetFHra(solar_time_dec);

  // SOME LOG FILE HERE

  if (_internal_state.acceleration_factor != _internal_state.acceleration_factor_old) {
    _internal_state.hra_old = (_internal_state.hra_old == 200 ? (std::fmod(fhra + 180, 360) - 180) : old_hra);
    _internal_state.hra_input = _internal_state.hra_old;
    _internal_state.acceleration_factor_old = _internal_state.acceleration_factor;
    _start_time = std::chrono::steady_clock::now();
  }

  double tmp_starting_hra = _internal_state.hra_enabled ? _internal_state.hra : 200;
  if (tmp_starting_hra != _internal_state.temp_hra_old) {
    _internal_state.temp_hra_old = tmp_starting_hra;
    if (!_internal_state.hra_enabled) {
      _internal_state.hra_old = fhra;
      _internal_state.hra_input = _internal_state.hra_old;
    } else {
      _internal_state.hra_input = tmp_starting_hra;
    }

    _start_time = std::chrono::steady_clock::now();
  }

  auto elapsed_time = std::chrono::duration_cast<std::chrono::minutes>(std::chrono::steady_clock::now() - _start_time);
  auto elapsed_time_double = std::chrono::duration_cast<std::chrono::duration<double>>(elapsed_time).count();
  double hra = std::fmod(_internal_state.hra_input + _internal_state.acceleration_factor * elapsed_time_double / 240 + 180, 360) - 180;

  double hra_rad = hra / 360.00 * 2.00 * PI;
  double lat_rad = _internal_state.latitude / 360.00 * 2.00 * PI;
  double alpha = asin(sin(delta_rad) * sin(lat_rad) + cos(delta_rad) * cos(lat_rad) * cos(hra_rad));
  double alpha_deg = alpha / 2.00 / PI * 360;
  double azimuth = acos((sin(delta_rad) * cos(lat_rad) - cos(delta_rad) * sin(lat_rad) * cos(hra_rad)) / cos(alpha));  // AZ RAD
  double azimuth_deg = azimuth / 2.00 / PI * 360;
  if (hra > 0) { azimuth_deg = 360.00 - azimuth_deg; }

  new_state.sun_azimuth = azimuth_deg;
  new_state.sun_elevation = alpha_deg;

  // IF TRACKING IS ON

  double hrar = hra;
  _internal_state.hrar_old = hrar;

  // AZ and EL from the reflector
  double hrar_rad = hrar / 360.00 * 2.00 * PI;
  alpha = asin(sin(delta_rad) * sin(lat_rad) + cos(delta_rad) * cos(lat_rad) * cos(hrar_rad));
  alpha_deg = alpha / 2.00 / PI * 360;
  azimuth = acos((sin(delta_rad) * cos(lat_rad) - cos(delta_rad) * sin(lat_rad) * cos(hrar_rad)) / cos(alpha));
  azimuth_deg = azimuth / 2.00 / PI * 360;
  if (hrar > 0) { azimuth_deg = 360.00 - azimuth_deg; }

  alpha_deg = alpha_deg < kElevationMin ? kElevationMin : alpha_deg;
  azimuth_deg = azimuth_deg > (180 + kAzimuthMax) ? (180 + kAzimuthMax) : azimuth_deg;
  azimuth_deg = azimuth_deg < (180 - kAzimuthMax) ? (180 - kAzimuthMax) : azimuth_deg;

  new_state.latitude = _internal_state.latitude;
  new_state.longitude = _internal_state.longitude;
  new_state.local_hours = local_time.tm_hour;
  new_state.local_minutes = local_time.tm_min;
  new_state.system_hours = (int)(std::floor(solar_time_dec));
  new_state.system_minutes = (int)(std::floor(std::fmod(solar_time_dec, 1) * 60.00));
  new_state.sun_ref_azimuth = azimuth_deg;
  new_state.sun_ref_elevation = alpha_deg;
  new_state.hra = hra;
  new_state.hrar = hrar;

  for (auto &reflector : _reflectors) {
    if (reflector.actual_status_azimuth != 0 || reflector.actual_status_elevation != 0) { /* TODO */
    }

    double az = azimuth_deg - reflector.calibration_azimuth_deg;
    double el = alpha_deg - reflector.calibration_elevation_deg;

    reflector.theoretical_position_azimuth_mm = -(((az / reflector.a1))) + reflector.calibration_azimuth_mm;
    reflector.theoretical_position_elevation_mm =
        -(((el / reflector.a3 - reflector.a2 * az / (reflector.a1 * reflector.a3)))) + reflector.calibration_elevation_mm;

    reflector.actual_position_azimuth_deg = utils::ConvertAzimuth_Mm2Deg(reflector, reflector.actual_position_azimuth_mm);
    reflector.actual_position_elevation_deg = utils::ConvertElevation_Mm2Deg(reflector, reflector.actual_position_elevation_mm);
    reflector.theoretical_position_azimuth_deg = utils::ConvertAzimuth_Mm2Deg(reflector, reflector.theoretical_position_azimuth_mm);
    reflector.theoretical_position_elevation_deg = utils::ConvertElevation_Mm2DegTh(reflector, reflector.theoretical_position_elevation_mm);
  }

  return new_state;
}

void ReflectorsController::ControllerThreadExecute() {
  const auto max_frame_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)) / 5;

  double hra_return[kNightSize];
  std::memcpy(hra_return, kHRAReturnDefault, kNightSize * sizeof(kHRAReturnDefault[0]));

  while (_is_working) {
    auto iteration_start = std::chrono::high_resolution_clock::now();
    try {
      ExecuteCommands();
    } catch (const std::runtime_error &ex) { _update_listener->OnUpdate(std::make_unique<ErrorUpdate>(std::string("Command execution error: ") + ex.what())); }

    if (_status != ControllerStatus::kTracking && _status != ControllerStatus::kOperating) {
      _status = _internal_state.is_tracking ? ControllerStatus::kTracking : ControllerStatus::kOperating;
      _update_listener->OnUpdate(std::make_unique<StatusUpdate>(_status));
    }

    auto new_state = UpdateData(_internal_state.hra);

    ProcessUpdates(new_state);

    auto frame_time = std::chrono::high_resolution_clock::now() - iteration_start;
    auto sleep_time = max_frame_time - frame_time;

    if (sleep_time.count() > 0) {
      // spdlog::info("Sleep controller for {} ns.", sleep_time.count());
      std::this_thread::sleep_for(sleep_time);
    }
  }
}

}  // namespace voltiris::controller
