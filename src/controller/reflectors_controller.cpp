#include "reflectors_controller.hpp"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>

#include <spdlog/spdlog.h>

#include <utils/frame_limiter.hpp>

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

double kAzimuthMax = 100;
double kElevationMin = 14.00;

}  // namespace

ReflectorsController::ReflectorsController() { _internal_state.start_time = std::chrono::steady_clock::now(); }

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
                                                                          false, _internal_state.starting_hra, _internal_state.acceleration_factor));
}

void ReflectorsController::ProcessCommand(const ValuesUpdateCommand &command) {
  spdlog::debug("ValuesUpdateCommand received.");
  _internal_state.new_hra = command.GetStartingHra();
  _internal_state.new_acceleration_factor = command.GetAccelerationFactor();
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
  ReflectorsControllerCommandsProcessor::ProcessInitializationCommand(command, _com_port, _reflectors);
}

void ReflectorsController::ProcessCommand(const ReadCommand &command) {
  spdlog::debug("ReadCommand received.");
  ReflectorsControllerCommandsProcessor::ProcessReadCommand(command, _com_port, _reflectors);
}

void ReflectorsController::ProcessCommand(const FlashCommand &command) {
  spdlog::debug("FlashCommand received.");
  ReflectorsControllerCommandsProcessor::ProcessFlashCommand(command, _com_port, _reflectors);
}

void ReflectorsController::ProcessCommand(const RebootCommand &command) {
  spdlog::debug("RebootCommand received.");
  ReflectorsControllerCommandsProcessor::ProcessRebootCommand(command, _com_port, _reflectors);
}

void ReflectorsController::ProcessCommand(const SetPositionCommand &command) {
  spdlog::debug("SetPositionCommand received.");
  ReflectorsControllerCommandsProcessor::ProcessSetPositionCommand(command, _com_port, _reflectors);
}

void ReflectorsController::ProcessCommand(const GoCommand &command) {
  spdlog::debug("GoCommand received.");
  ReflectorsControllerCommandsProcessor::ProcessGoCommand(command, _com_port, _reflectors);
}

void ReflectorsController::ProcessCommand(const ManualMoveCommand &command) {
  spdlog::debug("MoveCommand received.");
  ReflectorsControllerCommandsProcessor::ProcessManualMoveCommand(command, _com_port, _reflectors);
}

void ReflectorsController::ProcessCommand(const RequestConfigurationCommand &command) {
  spdlog::debug("RequestConfigurationCommand received.");
  _update_listener->OnUpdate(std::make_unique<CurrentConfigurationUpdate>(_internal_state.latitude, _internal_state.longitude, _internal_state.cycles_frequency,
                                                                          false, _internal_state.starting_hra, _internal_state.acceleration_factor));
  _update_listener->OnUpdate(std::make_unique<BusListUpdate>(utils::GetAvailablePorts()));

  _update_listener->OnUpdate(std::make_unique<ReflectorsSizeUpdate>(_reflectors.size()));
  for (std::size_t i = 0; i < _reflectors.size(); ++i) { _update_listener->OnUpdate(std::make_unique<ReflectorStateUpdate>(i, _reflectors[i])); }

  _update_listener->OnUpdate(_com_port == -1 ? std::unique_ptr<ReflectorsControllerUpdate>(new BusDisconnectedUpdate())
                                             : std::unique_ptr<ReflectorsControllerUpdate>(new BusConnectedUpdate()));

  _update_listener->OnUpdate(std::make_unique<StatusUpdate>(_status));
}

void ReflectorsController::ProcessCommand(const StartTrackingCommand &command) {
  spdlog::debug("StartTrackingCommand received.");

  _internal_state.is_tracking = true;
  _internal_state.is_night = false;
  _internal_state.is_night_returned = false;
}

void ReflectorsController::ProcessCommand(const StopTrackingCommand &command) {
  spdlog::debug("StopTrackingCommand received.");

  _internal_state.is_tracking = false;
  _internal_state.is_night = false;
  _internal_state.is_night_returned = false;
  for (auto &reflector : _reflectors) {
    reflector.should_be_calibrated = false;
    reflector.should_be_moved_azimuth.reset();
    reflector.should_be_moved_elevation.reset();
  }
}

void ReflectorsController::ProcessCommand(const AutomaticMoveCommand &command) {
  spdlog::debug("AutomaticMoveCommand received.");
  ReflectorsControllerCommandsProcessor::ProcessAutomaticMoveCommand(command, _reflectors);
}

void ReflectorsController::ProcessUpdates(const ReflectorsControllerIterationState &new_state) {
  if (_reflectors.size() != _reflectors_copy.size()) {
    _reflectors_copy.resize(_reflectors.size());
    _update_listener->OnUpdate(std::make_unique<ReflectorsSizeUpdate>(_reflectors.size()));
  }
  for (std::size_t i = 0; i < _reflectors.size(); ++i) {
    if (_reflectors_copy[i] != _reflectors[i]) {
      _reflectors_copy[i] = _reflectors[i];
      _update_listener->OnUpdate(std::make_unique<ReflectorStateUpdate>(i, _reflectors[i]));
    }
  }

  if (_iteration_state != new_state) {
    _iteration_state = new_state;

    _update_listener->OnUpdate(std::make_unique<ASTUpdate>(_iteration_state.latitude, _iteration_state.longitude, _iteration_state.local_hours,
                                                           _iteration_state.local_minutes, _iteration_state.sun_hours, _iteration_state.sun_minutes,
                                                           _iteration_state.sun_azimuth, _iteration_state.sun_elevation, _iteration_state.sun_ref_azimuth,
                                                           _iteration_state.sun_ref_elevation, _iteration_state.hra, _iteration_state.reflector_hra));
  }

  if (_status != _prev_status) {
    _prev_status = _status;
    _update_listener->OnUpdate(std::make_unique<StatusUpdate>(_status));
  }
}

ReflectorsControllerIterationState ReflectorsController::UpdateData(double old_hra) {
  ReflectorsControllerIterationState iteration_state;

  time_t time_t = utils::GetCurrentTimePoint();
  tm utc_time = utils::GetUTCTime(time_t);
  tm local_time = utils::GetLocalTime(time_t);

  double B = utils::GetB(utc_time.tm_mon, utc_time.tm_mday);
  double delta_rad = utils::GetDeltaRad(B);
  double solar_time_dec = utils::GetSolarTimeDec(utc_time.tm_hour, utc_time.tm_min, _internal_state.longitude, delta_rad, B);
  double fhra = utils::GetFHra(solar_time_dec);

  // SOME LOG FILE HERE

  if (_internal_state.starting_hra < 0) { _internal_state.starting_hra = std::fmod(fhra + 180, 360) - 180; }

  if (_internal_state.new_acceleration_factor.has_value()) {
    _internal_state.starting_hra = old_hra;
    _internal_state.acceleration_factor = _internal_state.new_acceleration_factor.value();
    _internal_state.new_acceleration_factor.reset();
    _internal_state.start_time = std::chrono::steady_clock::now();
  }

  if (_internal_state.new_hra.has_value()) {
    _internal_state.starting_hra = (_internal_state.new_hra.value() > -1 ? _internal_state.new_hra.value() : fhra);
    _internal_state.new_hra.reset();
    _internal_state.start_time = std::chrono::steady_clock::now();
  }

  auto elapsed_time = std::chrono::duration_cast<std::chrono::minutes>(std::chrono::steady_clock::now() - _internal_state.start_time);
  auto elapsed_time_double = std::chrono::duration_cast<std::chrono::duration<double>>(elapsed_time).count();
  double hra_deg = std::fmod(_internal_state.starting_hra + _internal_state.acceleration_factor * elapsed_time_double / 240 + 180, 360) - 180;

  double hra_rad = utils::GetRad(hra_deg);
  double lat_rad = utils::GetRad(_internal_state.latitude);
  double hra_alpha_rad = utils::GetAlphaRad(delta_rad, lat_rad, hra_rad);
  double hra_alpha_deg = utils::GetDeg(hra_alpha_rad);
  double hra_azimuth_rad = utils::GetAzimuthRad(delta_rad, lat_rad, hra_rad, hra_alpha_rad);
  double hra_azimuth_deg = utils::GetDeg(hra_azimuth_rad);
  hra_azimuth_deg = hra_deg > 0 ? 360.00 - hra_azimuth_deg : hra_azimuth_deg;

  iteration_state.delta_rad = delta_rad;
  iteration_state.lat_rad = lat_rad;
  iteration_state.sun_azimuth = hra_azimuth_deg;
  iteration_state.sun_elevation = hra_alpha_deg;

  double reflector_hra_deg = hra_deg;

  // AZ and EL from the reflector
  double reflector_hra_rad = utils::GetRad(reflector_hra_deg);
  double reflector_hra_alpha_rad = utils::GetAlphaRad(delta_rad, lat_rad, reflector_hra_rad);
  double reflector_hra_alpha_deg = utils::GetDeg(reflector_hra_alpha_rad);
  double reflector_hra_azimuth_rad = utils::GetAzimuthRad(delta_rad, lat_rad, reflector_hra_rad, reflector_hra_alpha_rad);
  double reflector_hra_azimuth_deg = utils::GetDeg(reflector_hra_azimuth_rad);

  reflector_hra_azimuth_deg = reflector_hra_deg > 0 ? 360.00 - reflector_hra_azimuth_deg : reflector_hra_azimuth_deg;
  reflector_hra_alpha_deg = reflector_hra_alpha_deg < kElevationMin ? kElevationMin : reflector_hra_alpha_deg;
  reflector_hra_azimuth_deg = reflector_hra_azimuth_deg > (180 + kAzimuthMax) ? (180 + kAzimuthMax) : reflector_hra_azimuth_deg;
  reflector_hra_azimuth_deg = reflector_hra_azimuth_deg < (180 - kAzimuthMax) ? (180 - kAzimuthMax) : reflector_hra_azimuth_deg;

  iteration_state.latitude = _internal_state.latitude;
  iteration_state.longitude = _internal_state.longitude;
  iteration_state.local_hours = local_time.tm_hour;
  iteration_state.local_minutes = local_time.tm_min;
  iteration_state.sun_hours = (int)(std::floor(solar_time_dec));
  iteration_state.sun_minutes = (int)(std::floor(std::fmod(solar_time_dec, 1) * 60.00));
  iteration_state.sun_ref_azimuth = reflector_hra_azimuth_deg;
  iteration_state.sun_ref_elevation = reflector_hra_alpha_deg;
  iteration_state.hra = hra_deg;
  iteration_state.reflector_hra = reflector_hra_deg;

  for (auto &reflector : _reflectors) {
    double az = reflector_hra_azimuth_deg - reflector.calibration_azimuth_deg;
    double el = reflector_hra_alpha_deg - reflector.calibration_elevation_deg;

    reflector.theoretical_position_azimuth_mm = utils::GetTheoreticalAzimuthMm(az, reflector.a1, reflector.calibration_azimuth_mm);
    reflector.theoretical_position_elevation_mm =
        utils::GetTheoreticalElevationMm(az, el, reflector.a1, reflector.a2, reflector.a3, reflector.calibration_elevation_mm);

    reflector.actual_position_azimuth_deg = utils::ConvertAzimuth_Mm2Deg(reflector, reflector.actual_position_azimuth_mm);
    reflector.actual_position_elevation_deg = utils::ConvertElevation_Mm2Deg(reflector, reflector.actual_position_elevation_mm);
    reflector.theoretical_position_azimuth_deg = utils::ConvertAzimuth_Mm2Deg(reflector, reflector.theoretical_position_azimuth_mm);
    reflector.theoretical_position_elevation_deg = utils::ConvertElevation_Mm2DegTh(reflector, reflector.theoretical_position_elevation_mm);
  }

  return iteration_state;
}

void ReflectorsController::ProcessTracking(const ReflectorsControllerIterationState &current_state) {
  _internal_state.is_night = ((current_state.sun_elevation < kElevationMin) || (current_state.sun_azimuth > (180 + kAzimuthMax)));
  _internal_state.is_night_returned = _internal_state.is_night && _internal_state.is_night_returned;

  if (_internal_state.is_night && !_internal_state.is_night_returned) {
    _internal_state.is_night_returned = true;
    // cycles to autocalibrate
    // td->autocalib_while_tracking = false;

    // Count cycles here

    // activate the 400-0 if arrived at the right number of cycles
    //      if (td->givenCycles > 0 && td->cycles_to_autorecalib >= td->givenCycles) {
    //        td->cycles_to_autorecalib = 0;
    //        td->autocalib_while_tracking = true;
    //      }

    double morning_hra_rad = utils::GetRad(-90);
    double morning_hra_alpha_rad = utils::GetAlphaRad(current_state.delta_rad, current_state.lat_rad, morning_hra_rad);
    double morning_hra_alpha_deg = utils::GetDeg(morning_hra_alpha_rad);
    double morning_hra_azimuth_rad = utils::GetAzimuthRad(current_state.delta_rad, current_state.lat_rad, morning_hra_rad, morning_hra_alpha_rad);
    double morning_hra_azimuth_deg = utils::GetDeg(morning_hra_azimuth_rad);

    morning_hra_alpha_deg = morning_hra_alpha_deg < kElevationMin ? kElevationMin : morning_hra_alpha_deg;
    morning_hra_azimuth_deg = morning_hra_azimuth_deg > (180 + kAzimuthMax) ? (180 + kAzimuthMax) : morning_hra_azimuth_deg;
    morning_hra_azimuth_deg = morning_hra_azimuth_deg < (180 - kAzimuthMax) ? (180 - kAzimuthMax) : morning_hra_azimuth_deg;

    for (auto &reflector : _reflectors) {
      reflector.should_be_moved_azimuth = morning_hra_azimuth_deg;
      reflector.should_be_moved_elevation = morning_hra_azimuth_deg;
    }

    spdlog::debug("Reflectors returned to morning position");
  }

  if (_internal_state.is_night) { return; }

  for (auto &reflector : _reflectors) {
    double delta_az = reflector.theoretical_position_azimuth_mm - reflector.actual_position_azimuth_mm;
    double delta_el = reflector.theoretical_position_elevation_mm - reflector.actual_position_elevation_mm;

    std::int32_t azimuth_sign = std::abs(delta_az) > 0 ? static_cast<std::int32_t>(delta_az / std::fabs(delta_az)) : 1;
    std::int32_t elevation_sign = std::abs(delta_el) > 0 ? static_cast<std::int32_t>(delta_el / std::fabs(delta_el)) : 1;

    double spacing = reflector.st_spacing;

    double target_az = reflector.theoretical_position_azimuth_mm + azimuth_sign * spacing / 2;
    double target_el = reflector.theoretical_position_elevation_mm + elevation_sign * spacing / 2;
    double az = std::abs(delta_az) > std::abs(spacing) ? target_az : reflector.actual_position_azimuth_mm;
    double el = std::abs(delta_el) > std::abs(spacing) ? target_el : reflector.actual_position_elevation_mm;

    if (std::abs(delta_az) > std::abs(spacing)) { reflector.should_be_moved_azimuth = az; }
    if (std::abs(delta_el) > std::abs(spacing)) { reflector.should_be_moved_elevation = el; }
  }
}

void ReflectorsController::ProcessSingleMovement() {
  for (auto &reflector : _reflectors) {
    if (reflector.should_be_calibrated) {
      ProcessSingleCalibrationMovement(reflector);
      return;
    }

    double target_az = reflector.should_be_moved_azimuth.has_value() ? reflector.should_be_moved_azimuth.value() : reflector.actual_position_azimuth_mm;
    double target_el = reflector.should_be_moved_elevation.has_value() ? reflector.should_be_moved_elevation.value() : reflector.actual_position_elevation_mm;

    if (!reflector.should_be_moved_azimuth.has_value() && !reflector.should_be_moved_elevation.has_value()) { continue; }

    utils::StepMoveTo(_com_port, reflector, target_az, target_el);

    if (std::abs(reflector.actual_position_azimuth_mm - target_az) < 1.0) { reflector.should_be_moved_azimuth.reset(); }
    if (std::abs(reflector.actual_position_elevation_mm - target_el) < 1.0) { reflector.should_be_moved_elevation.reset(); }

    utils::ReadPositioningData(_com_port, reflector);

    ReflectorsControllerCommandsProcessor::ProcessReadCommand(ReadCommand{}, _com_port, _reflectors);

    return;
  }
}

void ReflectorsController::ProcessSingleCalibrationMovement(ReflectorState &reflector) {
  static const std::int16_t kCalibrationCycles = 3;
  static const std::int16_t kCalibrationDoubleclickCycle = 2;

  bool result = false;

  result &= utils::ReadPositioningData(_com_port, reflector);

  bool is_azimuth_min = reflector.actual_status_azimuth == 1;
  bool is_azimuth_max = reflector.actual_status_azimuth == 2;
  bool is_elevation_min = reflector.actual_status_elevation == 1;
  bool is_elevation_max = reflector.actual_status_elevation == 2;

  if (is_azimuth_min || is_azimuth_max || is_elevation_min || is_elevation_max) {
    double temp_pos_az = is_azimuth_min || is_azimuth_max ? 200 : reflector.actual_position_azimuth_mm;
    double temp_pos_el = is_elevation_min || is_elevation_max ? 200 : reflector.actual_position_elevation_mm;
    result &= utils::SetPosition(_com_port, reflector, temp_pos_az, temp_pos_el);
    result &= utils::Flash(_com_port, reflector);
    result &= utils::Reboot(_com_port, reflector);
    result &= utils::WakeUp(_com_port, reflector);
    result &= utils::ReadPositioningData(_com_port, reflector);
    return;
  }

  reflector.azimuth_is_max = reflector.actual_status_azimuth == 3 || reflector.azimuth_is_max;
  reflector.elevation_is_min = reflector.actual_status_elevation == 3 || reflector.actual_status_elevation == 4 || reflector.elevation_is_min;

  double az_delta = reflector.azimuth_is_max ? 0. : -10.;
  double el_delta = reflector.elevation_is_min ? 0. : 10.;

  if (reflector.azimuth_is_max && reflector.elevation_is_min) {
    reflector.azimuth_is_max = false;
    reflector.elevation_is_min = false;
    result = utils::SetPosition(_com_port, reflector, 0, 400);
    result &= utils::Flash(_com_port, reflector);
    result &= utils::Reboot(_com_port, reflector);
    result &= utils::ReadPositioningData(_com_port, reflector);
    ++reflector.calibration_cycles;
    spdlog::info("For reflector {} calibration cycle increased, current value: {}", reflector.id, reflector.calibration_cycles);
    reflector.should_be_calibrated = kCalibrationCycles == reflector.calibration_cycles ? false : reflector.should_be_calibrated;
  } else if (reflector.azimuth_is_max) {
    result &= utils::Flash(_com_port, reflector);
    result &= utils::Reboot(_com_port, reflector);
    result &= utils::ReadPositioningData(_com_port, reflector);
    az_delta = 10;
    reflector.azimuth_is_max = false;
  } else if (reflector.elevation_is_min) {
    result &= utils::Flash(_com_port, reflector);
    result &= utils::Reboot(_com_port, reflector);
    result &= utils::ReadPositioningData(_com_port, reflector);
    el_delta = -10;
    reflector.elevation_is_min = false;
  }

  if (reflector.should_be_calibrated) {
    if (reflector.calibration_cycles == kCalibrationDoubleclickCycle && !reflector.calibration_doubleclicked) {
      az_delta = 20;
      el_delta = -20;
    }

    result = utils::StepMoveOn(_com_port, reflector, az_delta, el_delta);

    reflector.calibration_doubleclicked = reflector.calibration_cycles == kCalibrationDoubleclickCycle || reflector.calibration_doubleclicked;
  }
}

void ReflectorsController::ControllerThreadExecute() {
  voltiris::utils::FrameLimiter frame_limiter(5);

  while (_is_working) {
    frame_limiter.StartFrame();

    try {
      ExecuteCommands();
    } catch (const std::runtime_error &ex) { _update_listener->OnUpdate(std::make_unique<ErrorUpdate>(std::string("Command execution error: ") + ex.what())); }

    _status = _internal_state.is_tracking ? ControllerStatus::kTracking : ControllerStatus::kOperating;

    auto new_state = UpdateData(_internal_state.starting_hra);

    ProcessUpdates(new_state);

    if (_internal_state.is_tracking) {
      try {
        ProcessTracking(new_state);
      } catch (const std::runtime_error &ex) { _update_listener->OnUpdate(std::make_unique<ErrorUpdate>(std::string("Tracking error: ") + ex.what())); }
    }

    ProcessUpdates(new_state);

    try {
      ProcessSingleMovement();
    } catch (const std::runtime_error &ex) { _update_listener->OnUpdate(std::make_unique<ErrorUpdate>(std::string("Movement error: ") + ex.what())); }

    frame_limiter.LimitFrame();
  }
}

}  // namespace voltiris::controller
