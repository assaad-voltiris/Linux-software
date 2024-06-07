#include "reflectors_controller.hpp"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>

#include <spdlog/spdlog.h>

#include <controller/commands/connect_command.hpp>
#include <controller/commands/disconnect_command.hpp>
#include <controller/commands/flash_command.hpp>
#include <controller/commands/go_command.hpp>
#include <controller/commands/initialization_command.hpp>
#include <controller/commands/load_configuration_command.hpp>
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

static double kMaxSecu = 39.0;

const int kNightSize = 24;
const double kHRAReturnDefault[kNightSize] = {90.0, 82.5, 75,    67.5,  60.0,  52.5,  45.0,  37.5,  30.0,  22.5,  15.0,  7.5,
                                              1.0,  -7.5, -15.0, -22.5, -30.0, -37.5, -45.0, -52.5, -60.0, -67.5, -75.0, -82.5};

double kAzimuthMax = 80;
double kElevationMin = 16.00;

constexpr auto *kRS485Device = "/dev/ttyS5";
#define BAUDRATE B9600   // Baudrate (can be changed as needed)
#define BUFFER_SIZE 255  // Buffer size for sending and receiving data

}  // namespace

ReflectorsController::ReflectorsController() {}

void ReflectorsController::Start() {
  _is_working = true;

  _update_listener->OnBusListUpdate(utils::GetAvailablePorts());

  _controller_thread = std::thread(&ReflectorsController::ControllerThreadExecute, this);
}

void ReflectorsController::Stop() {
  if (_com_port != -1) {
    utils::Disconnect(_com_port);
    _update_listener->OnBusDisconnected();
  }

  _is_working = false;
  _controller_thread.join();
}

void ReflectorsController::OnCommand(std::unique_ptr<ReflectorsControllerCommand> command) {
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
  spdlog::debug("ProcessCommand -- LoadConfigurationCommand");

  _reflectors = utils::LoadReflectorsFromConfigurationFile(command.GetFilePath());

  _update_listener->OnRDReflectorsSize(_reflectors.size());
  for (std::size_t i = 0; i < _reflectors.size(); ++i) { _update_listener->OnRDReflectorState(i, _reflectors[i]); }
}

void ReflectorsController::ProcessCommand(const ValuesUpdateCommand &command) {
  spdlog::debug("ProcessCommand -- ValuesUpdateCommand");

  for (const auto &update : command.GetUpdates()) {
    switch (update.value_id) {
      case ValuesUpdateCommand::ValueId::kAccelerationFactor: {
        _acceleration_factor = update.value_double;
        break;
      }
      case ValuesUpdateCommand::ValueId::kStartingHra: {
        _hra = update.value_double;
        break;
      }
      case ValuesUpdateCommand::ValueId::kCycleFrequency: {
        _cycles_frequency = update.value_double;
        break;
      }
      case ValuesUpdateCommand::ValueId::kLatitude: {
        _latitude = update.value_double;
        break;
      }
      case ValuesUpdateCommand::ValueId::kLongitude: {
        _longitude = update.value_double;
        break;
      }
      case ValuesUpdateCommand::ValueId::kStartingHraEnabled:
        _hra_enabled = update.value_bool;
        break;
    }
  }
}

void ReflectorsController::ProcessCommand(const ConnectCommand &command) {
  spdlog::debug("ProcessCommand -- ConnectCommand");

  if (_com_port != -1) { throw std::runtime_error("Port already connected."); }

  _com_port = utils::Connect(command.GetBusName());

  _update_listener->OnBusConnected();

  //  if (command.GetIsConnect()) {
  //    if (_com_port != -1) {
  //      _data_observer->OnError("Port already connected");
  //      return;
  //    }
  //
  //
  //
  //    /// TODO remove Test send
  //    // Get user input for message to send
  //    char send_buffer[] = "FromVoltiris\n";
  //    // send_buffer[strcspn(send_buffer, "\n")] = '\0';
  //
  //    // Write data
  //    if (write(_com_port, send_buffer, strlen(send_buffer)) == -1) {
  //      _data_observer->OnError("Write error");
  //      close(_com_port);
  //    }
  //  } else {
  //    close(_com_port);
  //    _com_port = -1;
  //  }
}

void ReflectorsController::ProcessCommand(const DisconnectCommand &command) {
  spdlog::debug("ProcessCommand -- DisconnectCommand");

  utils::Disconnect(_com_port);
  _com_port = -1;
  _update_listener->OnBusDisconnected();
}

void ReflectorsController::ProcessCommand(const InitializationCommand &command) {
  spdlog::debug("ProcessCommand -- InitializationCommand");

  if (_com_port == -1) { throw std::runtime_error("Reflectors not connected."); }

  bool result = true;
  for (auto &reflector : _reflectors) {
    result &= utils::WakeUp(_com_port, reflector);
    result &= utils::Initialize(_com_port, reflector);
    result &= utils::ReadPositioningData(_com_port, reflector);
    result &= utils::SetHall(_com_port, reflector);
  }

  if (!result) { throw std::runtime_error("Reflectors initialization error."); }
}

void ReflectorsController::ProcessCommand(const ReadCommand &command) {
  spdlog::debug("ProcessCommand -- ReadCommand");

  if (_com_port == -1) { throw std::runtime_error("Reflectors not connected."); }

  bool result = true;
  for (auto &reflector : _reflectors) {
    result &= utils::WakeUp(_com_port, reflector);
    result &= utils::ReadPositioningData(_com_port, reflector);
  }

  if (!result) { throw std::runtime_error("Reflectors initialization error."); }
}

void ReflectorsController::ProcessCommand(const FlashCommand &command) {
  spdlog::debug("ProcessCommand -- FlashCommand");

  if (_com_port == -1) { throw std::runtime_error("Reflectors not connected."); }

  bool result = true;
  for (auto &reflector : _reflectors) {
    result &= utils::WakeUp(_com_port, reflector);
    result &= utils::Flash(_com_port, reflector);
  }

  if (!result) { throw std::runtime_error("Reflectors initialization error."); }
}

void ReflectorsController::ProcessCommand(const RebootCommand &command) {
  spdlog::debug("ProcessCommand -- RebootCommand");

  if (_com_port == -1) { throw std::runtime_error("Reflectors not connected."); }

  bool result = true;
  for (auto &reflector : _reflectors) {
    result &= utils::WakeUp(_com_port, reflector);
    result &= utils::Reboot(_com_port, reflector);
    result &= utils::ReadPositioningData(_com_port, reflector);
  }

  if (!result) { throw std::runtime_error("Reflectors initialization error."); }
}

void ReflectorsController::ProcessCommand(const SetPositionCommand &command) {
  spdlog::debug("ProcessCommand -- SetPositionCommand");

  if (_com_port == -1) { throw std::runtime_error("Reflectors not connected."); }

  bool result = true;

  result &= utils::WakeUp(_com_port, _reflectors[command.GetReflectorIndex()]);
  result &= utils::SetPosition(_com_port, _reflectors[command.GetReflectorIndex()], command.GetAzimuth(), command.GetElevation());
  result &= utils::ReadPositioningData(_com_port, _reflectors[command.GetReflectorIndex()]);

  if (!result) { throw std::runtime_error("Reflectors initialization error."); }
}
void ReflectorsController::ProcessCommand(const GoCommand &command) {
  spdlog::debug("ProcessCommand -- GoCommand");

  if (_com_port == -1) { throw std::runtime_error("Reflectors not connected."); }

  bool result = true;

  auto &reflector = _reflectors[command.GetReflectorIndex()];

  double azimuth = command.GetAzimuth();
  double elevation = command.GetElevation();

  do {
    result &= utils::ReadPositioningData(_com_port, reflector);
    if (!result) { throw std::runtime_error("Reflectors initial status reading error."); }

    azimuth = command.GetAzimuth();
    elevation = command.GetElevation();

    if (std::abs(reflector.actual_position_azimuth_mm - command.GetAzimuth()) > kMaxSecu) {
      azimuth = reflector.actual_position_azimuth_mm + (command.GetAzimuth() - reflector.actual_position_azimuth_mm) /
                                                           std::abs(command.GetAzimuth() - reflector.actual_position_azimuth_mm) * (kMaxSecu - 1.00);
    }
    if (std::abs(reflector.actual_position_elevation_mm - command.GetElevation()) > kMaxSecu) {
      elevation = reflector.actual_position_elevation_mm + (command.GetElevation() - reflector.actual_position_elevation_mm) /
                                                               std::abs(command.GetElevation() - reflector.actual_position_elevation_mm) * (kMaxSecu - 1.00);
    }

    result &= utils::Move(_com_port, reflector, azimuth, elevation);
    result &= utils::ReadPositioningData(_com_port, reflector);
    if (!result) { throw std::runtime_error("Reflectors movement error."); }
  } while (azimuth != command.GetAzimuth() || (elevation != command.GetElevation()));
}

void ReflectorsController::ControllerThreadExecute() {
  const auto max_frame_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)) / 5;

  double hra = 0;
  double hrar = 0;

  double hra_return[kNightSize];
  std::memcpy(hra_return, kHRAReturnDefault, kNightSize * sizeof(kHRAReturnDefault[0]));
  double hra_star = 90;

  auto start = std::chrono::high_resolution_clock::now();

  while (_is_working) {
    try {
      ExecuteCommands();
    } catch (const std::runtime_error &ex) { _update_listener->OnError(std::string("Command execution error: ") + ex.what()); }

    time_t time_t = utils::GetCurrentTimePoint();
    tm utc_time = utils::GetUTCTime(time_t);
    tm local_time = utils::GetLocalTime(time_t);

    double B = utils::GetB(utc_time.tm_mon, utc_time.tm_mday);
    double delta_rad = utils::GetDeltaRad(B);
    double solar_time_dec = utils::GetSolarTimeDec(utc_time.tm_mon, utc_time.tm_mday, _longitude, delta_rad, B);
    double fhra = utils::GetFHra(solar_time_dec);

    // SOME LOG FILE HERE

    if (_acceleration_factor != _acceleration_factor_old) {
      _hra_old = (_hra_old == 200 ? (std::fmod(fhra + 180, 360) - 180) : hra);
      _hra_input = _hra_old;
      _acceleration_factor_old = _acceleration_factor;
      _start_time = std::chrono::steady_clock::now();
    }

    double tmp_starting_hra = _hra_enabled ? _hra : 200;
    if (tmp_starting_hra != _temp_hra_old) {
      _temp_hra_old = tmp_starting_hra;
      if (!_hra_enabled) {
        _hra_old = fhra;
        _hra_input = _hra_old;
      } else {
        _hra_input = tmp_starting_hra;
      }

      _start_time = std::chrono::steady_clock::now();
    }

    auto elapsed_time = std::chrono::duration_cast<std::chrono::minutes>(std::chrono::steady_clock::now() - _start_time);
    auto elapsed_time_double = std::chrono::duration_cast<std::chrono::duration<double>>(elapsed_time).count();
    hra = std::fmod(_hra_input + _acceleration_factor * elapsed_time_double / 240 + 180, 360) - 180;

    double hra_rad = hra / 360.00 * 2.00 * M_PI;
    double lat_rad = _latitude / 360.00 * 2.00 * M_PI;
    double alpha = asin(sin(delta_rad) * sin(lat_rad) + cos(delta_rad) * cos(lat_rad) * cos(hra_rad));
    double alpha_deg = alpha / 2.00 / M_PI * 360;
    double azimuth = acos((sin(delta_rad) * cos(lat_rad) - cos(delta_rad) * sin(lat_rad) * cos(hra_rad)) / cos(alpha));  // AZ RAD
    double azimuth_deg = azimuth / 2.00 / M_PI * 360;
    if (hra > 0) { azimuth_deg = 360.00 - azimuth_deg; }

    _update_listener->OnASTLocalTime(local_time.tm_hour, local_time.tm_min);
    _update_listener->OnASTSystemTime((int)(std::floor(solar_time_dec)), (int)(std::floor(std::fmod(solar_time_dec, 1) * 60.00)));
    _update_listener->OnASTSunAzimuth(azimuth_deg);
    _update_listener->OnASTSunElevation(alpha_deg);

    // IF TRACKING IS ON

    hrar = hra;
    _hrar_old = hrar;

    // AZ and EL from the reflector
    double hrar_rad = hrar / 360.00 * 2.00 * M_PI;
    alpha = asin(sin(delta_rad) * sin(lat_rad) + cos(delta_rad) * cos(lat_rad) * cos(hrar_rad));
    alpha_deg = alpha / 2.00 / M_PI * 360;
    azimuth = acos((sin(delta_rad) * cos(lat_rad) - cos(delta_rad) * sin(lat_rad) * cos(hrar_rad)) / cos(alpha));
    azimuth_deg = azimuth / 2.00 / M_PI * 360;
    if (hrar > 0) { azimuth_deg = 360.00 - azimuth_deg; }

    alpha_deg = alpha_deg < kElevationMin ? kElevationMin : alpha_deg;
    azimuth_deg = azimuth_deg > (180 + kAzimuthMax) ? (180 + kAzimuthMax) : azimuth_deg;
    azimuth_deg = azimuth_deg < (180 - kAzimuthMax) ? (180 - kAzimuthMax) : azimuth_deg;

    _update_listener->OnASTSunRefAzimuth(azimuth_deg);
    _update_listener->OnASTSunRefElevation(alpha_deg);

    _update_listener->OnASTHra(hra);
    _update_listener->OnASTHrar(hrar);

    for (std::size_t i = 0; i < _reflectors.size(); ++i) {
      auto &reflector = _reflectors[i];

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

      _update_listener->OnRDReflectorState(i, reflector);
    }

    auto frame_time = std::chrono::high_resolution_clock::now() - start;
    auto sleep_time = max_frame_time - frame_time;

    if (sleep_time.count() > 0) {
      // spdlog::info("Sleep controller for {} ms.", sleep_time.count());
      std::this_thread::sleep_for(sleep_time);
    }
  }
}

}  // namespace voltiris::controller
