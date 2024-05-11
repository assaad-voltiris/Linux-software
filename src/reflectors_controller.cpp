#include "reflectors_controller.hpp"

#include <spdlog/spdlog.h>

#include <chrono>
#include <commands/load_configuration_command.hpp>
#include <ctime>
#include <fstream>
#include <iostream>
#include <resources.hpp>

namespace voltiris {

#define M_PI 3.141592653589

namespace {
// const double kPi = 3.141592653589793238463;

const int kNightSize = 24;
const double kHRAReturnDefault[kNightSize] = {90.0, 82.5, 75,    67.5,  60.0,  52.5,  45.0,  37.5,  30.0,  22.5,  15.0,  7.5,
                                              1.0,  -7.5, -15.0, -22.5, -30.0, -37.5, -45.0, -52.5, -60.0, -67.5, -75.0, -82.5};

double kAzimuthMax = 80;
;
double kElevationMin = 16.00;

}  // namespace

ReflectorsController::ReflectorsController() {}

void ReflectorsController::Start() {
  _is_working = true;
  _controller_thread = std::thread(&ReflectorsController::ControllerThreadExecute, this);
}

void ReflectorsController::Stop() {
  _is_working = false;
  _controller_thread.join();
}

void voltiris::ReflectorsController::OnCommand(std::unique_ptr<ReflectorsControllerCommand> command) {
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
  std::ifstream configuration_file(command.GetFilePath());

  if (!configuration_file.is_open()) {
    _data_observer->OnError("File Not Opened! File path: " + command.GetFilePath());
    return;
  }

  std::string line;
  while (std::getline(configuration_file, line)) {
    if (line.empty()) { continue; }

    _rs_adapters[command.GetBusIndex()]._reflectors.emplace_back();
    ArduinoMega &new_reflector = _rs_adapters[command.GetBusIndex()]._reflectors.back();

    int ref_id_s, com_id_s, line_num_s;
    double a1_s, a2_s, a3_s, hall_factor_s, azimuth_mm_calib_s, elevation_mm_calib_s, azimuth_deg_calib_s, elevation_deg_calib_s, latitude_s, longitude_s,
        st_spacing_s;

    // clang-format off
    // refId : comId : lineNum : a1 : a2 : a3 : HallFactor: AZmm_calib : ELmm_calib : AZdeg_calib : ELdeg_calib : Latitude : Longitude : STspacing;
    std::sscanf(line.c_str(),"%d : %d : %d : %lf : %lf : %lf : %lf : %lf : %lf : %lf : %lf : %lf : %lf : %lf;",
                &ref_id_s, &com_id_s, &line_num_s, &a1_s, &a2_s, &a3_s, &hall_factor_s, &azimuth_mm_calib_s, &elevation_mm_calib_s, &azimuth_deg_calib_s, &elevation_deg_calib_s, &latitude_s, &longitude_s, &st_spacing_s);

    new_reflector.ref_id                                  = ref_id_s;
    new_reflector.com_id                                  = com_id_s;
    new_reflector.line_num                                = line_num_s;
    new_reflector.reflector_actual_position_elevation     = 999;                      // Current position EL(mm) of reflector
    new_reflector.reflector_actual_position_azimuth       = 888;                      // Current position AZ(mm) of reflector
    new_reflector.reflector_actual_position_elevation_old = 999;                      // Old position EL(mm) of reflector--if reboot
    new_reflector.reflector_actual_position_azimuth_old   = 888;                      // Old position AZ(mm) of reflector--if reboot
    new_reflector.reflector_calibration_motor_elevation   = 0;                        // Old position EL(mm) of reflector--if reboot
    new_reflector.reflector_calibration_motor_azimuth     = 0;                        // Old position AZ(mm) of reflector--if reboot
    new_reflector.reflector_actual_status_azimuth         = 9;                        // Current error status of reflector
    new_reflector.reflector_actual_status_elevation       = 9;                        // Current error status of reflector
    new_reflector.reflector_th_position_elevation         = 0;                        // Current theoretical position EL(mm) of reflector
    new_reflector.reflector_th_position_azimuth           = 0;                        // Current position AZ(mm) of reflector
    new_reflector.reflector_th_position_elevation_old     = 0;                        // Current theoretical position EL(mm) of reflector
    new_reflector.reflector_th_position_azimuth_old       = 0;                        // Current position AZ(mm) of reflector
    new_reflector.reflector_a1                            = a1_s;                     // Mechanical factors a1 of reflector
    new_reflector.reflector_a2                            = a2_s;                     // Mechanical factors  a2 of reflector
    new_reflector.reflector_a3                            = a3_s;                     // Mechanical factors  a3 of reflector
    new_reflector.reflector_calibration_elevation_mm      = elevation_mm_calib_s;     // Calibration position EL(mm) of reflector
    new_reflector.reflector_calibration_azimuth_mm        = azimuth_mm_calib_s;       // Calibration position AZ(mm) of reflector
    new_reflector.reflector_calibration_elevation_deg     = elevation_deg_calib_s;    // Calibration position EL(degree) of reflector
    new_reflector.reflector_calibration_azimuth_deg       = azimuth_deg_calib_s;      // Calibration position AZ(degree) of reflector
    new_reflector.reflector_hall_spacing                  = hall_factor_s;            // Hall factor
    new_reflector.reflector_latitude                      = latitude_s;               // Latitude
    new_reflector.reflector_longitude                     = longitude_s;              // Longitude
    new_reflector.reflector_firmware                      = 0.0;                      // Firmware
    new_reflector.reflector_id_wafer                      = 0;                        // unique ID1
    new_reflector.reflector_id_die                        = 0;                        // unique // ID2
    new_reflector.st_spacing                              = st_spacing_s;
    // clang-format on
  }
  configuration_file.close();
}

void ReflectorsController::ProcessCommand(const ValuesUpdateCommand &command) {
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

void ReflectorsController::ControllerThreadExecute() {
  double hra = 0;
  double hrar = 0;

  double hra_return[kNightSize];
  std::memcpy(hra_return, kHRAReturnDefault, kNightSize * sizeof(kHRAReturnDefault[0]));
  double hra_star = 90;

  while (_is_working) {
    ExecuteCommands();

    time_t time_t;
    struct tm* utc_time {};
    struct tm* local_time {};

    time(&time_t);
    local_time = localtime(&time_t);
    utc_time = gmtime(&time_t);

    double d = 30.44 * ((double)utc_time->tm_mon) + (double)utc_time->tm_mday;
    double B = 2.00 * M_PI * (d - 81.00) / 365.0;
    double delta = 23.45 * std::sin(B);
    double delta_rad = delta / 360.00 * 2.00 * M_PI;
    double EoT = 9.87 * std::sin(2 * B) - 7.53 * std::cos(B) - 1.5 * std::sin(B);
    double time_diff_long_hour = (4.00 * _longitude + EoT) / 60.00;
    double time_utc_dec = (double)utc_time->tm_hour + (double)utc_time->tm_min / 60.00;
    double solar_time_dec = time_utc_dec + time_diff_long_hour;
    double fhra = 15.00 * (solar_time_dec - 12.00);

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

    spdlog::info("Local time: {}h{}m; System time: {}h{}m", local_time->tm_hour, local_time->tm_min, (int)(std::floor(solar_time_dec)), (int)(std::floor(std::fmod(solar_time_dec, 1) * 60.00)));
    _data_observer->OnASTLocalTime(local_time->tm_hour, local_time->tm_min);
    _data_observer->OnASTSystemTime((int)(std::floor(solar_time_dec)), (int)(std::floor(std::fmod(solar_time_dec, 1) * 60.00)));
    _data_observer->OnASTSunAzimuth(azimuth_deg);
    _data_observer->OnASTSunElevation(alpha_deg);

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

    _data_observer->OnASTSunRefAzimuth(azimuth_deg);
    _data_observer->OnASTSunRefElevation(alpha_deg);

    _data_observer->OnASTHra(hra);
    _data_observer->OnASTHrar(hrar);

    for (auto &rs_adapter : _rs_adapters) {
      for (auto &reflector : rs_adapter._reflectors) {
        if (reflector.reflector_actual_status_azimuth != 0 || reflector.reflector_actual_status_elevation != 0) { /* TODO */
        }

        double az = azimuth_deg - reflector.reflector_calibration_azimuth_deg;
        double el = alpha_deg - reflector.reflector_calibration_elevation_deg;

        reflector.reflector_th_position_azimuth = -(((az / reflector.reflector_a1))) + reflector.reflector_calibration_azimuth_mm;
        reflector.reflector_th_position_elevation =
            -(((el / reflector.reflector_a3 - reflector.reflector_a2 * az / (reflector.reflector_a1 * reflector.reflector_a3)))) +
            reflector.reflector_calibration_elevation_mm;

        _data_observer->OnRDReflectorStatusAzimuth(reflector.ref_id - 1, reflector.reflector_actual_status_azimuth);
        _data_observer->OnRDReflectorStatusElevation(reflector.ref_id - 1, reflector.reflector_actual_status_elevation);

        _data_observer->OnRDReflectorPositionAzimuthMm(reflector.ref_id - 1, reflector.reflector_actual_position_azimuth);
        _data_observer->OnRDReflectorPositionAzimuthDeg(reflector.ref_id - 1, reflector.ConvertAZ_Mm2Deg(reflector.reflector_actual_position_azimuth));

        _data_observer->OnRDReflectorPositionElevationMm(reflector.ref_id - 1, reflector.reflector_actual_position_elevation);
        _data_observer->OnRDReflectorPositionElevationDeg(reflector.ref_id - 1, reflector.ConvertEL_Mm2Deg(reflector.reflector_actual_position_elevation));

        _data_observer->OnRDReflectorThPositionAzimuthMm(reflector.ref_id - 1, reflector.reflector_th_position_azimuth);
        _data_observer->OnRDReflectorThPositionAzimuthDeg(reflector.ref_id - 1, reflector.ConvertAZ_Mm2Deg(reflector.reflector_th_position_azimuth));

        _data_observer->OnRDReflectorThPositionElevationMm(reflector.ref_id - 1, reflector.reflector_th_position_elevation);
        _data_observer->OnRDReflectorThPositionElevationDeg(reflector.ref_id - 1, reflector.ConvertEL_Mm2DegTh(reflector.reflector_th_position_elevation));
      }
    }
  }
}

}  // namespace voltiris
