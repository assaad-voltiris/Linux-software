#include "reflector.hpp"

#include <fstream>
#include <sstream>

#include <spdlog/spdlog.h>

#include <controller/reflector_state.hpp>
#include <controller/utils/communication.hpp>

namespace voltiris::controller::utils {

ReflectorState LoadReflectorFromConfiguration(const std::string& config) {
  ReflectorState reflector;

  int ref_id_s, com_id_s, line_num_s;
  double a1_s, a2_s, a3_s, hall_factor_s, azimuth_mm_calib_s, elevation_mm_calib_s, azimuth_deg_calib_s, elevation_deg_calib_s, latitude_s, longitude_s,
      st_spacing_s;

  // clang-format off
  // refId : comId : lineNum : a1 : a2 : a3 : HallFactor: AZmm_calib : ELmm_calib : AZdeg_calib : ELdeg_calib : Latitude : Longitude : STspacing;
  std::sscanf(config.c_str(),"%d : %d : %d : %lf : %lf : %lf : %lf : %lf : %lf : %lf : %lf : %lf : %lf : %lf;",
              &ref_id_s, &com_id_s, &line_num_s, &a1_s, &a2_s, &a3_s, &hall_factor_s, &azimuth_mm_calib_s, &elevation_mm_calib_s, &azimuth_deg_calib_s, &elevation_deg_calib_s, &latitude_s, &longitude_s, &st_spacing_s);

  reflector.id                                      = ref_id_s;
  reflector.com_id                                  = com_id_s;
  reflector.line_num                                = line_num_s;
  reflector.actual_position_elevation_mm            = 999;                      // Current position EL(mm) of reflector
  reflector.actual_position_azimuth_mm              = 888;                      // Current position AZ(mm) of reflector
  reflector.actual_position_elevation_deg           = 888;                      // Current position AZ(deg) of reflector
  reflector.actual_position_azimuth_deg             = 888;                      // Current position AZ(deg) of reflector
  reflector.actual_position_elevation_previous      = 999;                      // Old position EL(mm) of reflector--if reboot
  reflector.actual_position_azimuth_previous        = 888;                      // Old position AZ(mm) of reflector--if reboot
  reflector.calibration_motor_elevation             = 0;                        // Old position EL(mm) of reflector--if reboot
  reflector.calibration_motor_azimuth               = 0;                        // Old position AZ(mm) of reflector--if reboot
  reflector.actual_status_azimuth                   = 9;                        // Current error status of reflector
  reflector.actual_status_elevation                 = 9;                        // Current error status of reflector
  reflector.theoretical_position_elevation_mm       = 0;                        // Current theoretical position EL(mm) of reflector
  reflector.theoretical_position_azimuth_mm         = 0;                        // Current position AZ(mm) of reflector
  reflector.theoretical_position_elevation_deg      = 0;                        // Current theoretical position EL(deg) of reflector
  reflector.theoretical_position_azimuth_deg        = 0;                        // Current position AZ(deg) of reflector
  reflector.theoretical_position_elevation_previous = 0;                        // Current theoretical position EL(mm) of reflector
  reflector.theoretical_position_azimuth_previous   = 0;                        // Current position AZ(mm) of reflector
  reflector.a1                                      = a1_s;                     // Mechanical factors a1 of reflector
  reflector.a2                                      = a2_s;                     // Mechanical factors  a2 of reflector
  reflector.a3                                      = a3_s;                     // Mechanical factors  a3 of reflector
  reflector.calibration_elevation_mm                = elevation_mm_calib_s;     // Calibration position EL(mm) of reflector
  reflector.calibration_azimuth_mm                  = azimuth_mm_calib_s;       // Calibration position AZ(mm) of reflector
  reflector.calibration_elevation_deg               = elevation_deg_calib_s;    // Calibration position EL(degree) of reflector
  reflector.calibration_azimuth_deg                 = azimuth_deg_calib_s;      // Calibration position AZ(degree) of reflector
  reflector.hall_spacing                            = hall_factor_s;            // Hall factor
  reflector.latitude                                = latitude_s;               // Latitude
  reflector.longitude                               = longitude_s;              // Longitude
  reflector.firmware                                = 0.0;                      // Firmware
  reflector.id_wafer                                = 0;                        // unique ID1
  reflector.id_die                                  = 0;                        // unique // ID2
  reflector.st_spacing                              = st_spacing_s;
  // clang-format on

  return reflector;
}

std::vector<ReflectorState> LoadReflectorsFromConfigurationFile(const std::string& config_file) {
  std::ifstream configuration_file(config_file);

  if (!configuration_file.is_open()) { throw std::runtime_error("File opening issue. File path: " + config_file); }

  std::vector<ReflectorState> result;

  std::string line;
  while (std::getline(configuration_file, line)) {
    if (line.empty()) { continue; }

    result.emplace_back(LoadReflectorFromConfiguration(line));
  }
  configuration_file.close();

  std::sort(result.begin(), result.end(), [](const ReflectorState& lhs, const ReflectorState& rhs) { return lhs.id < rhs.id; });

  return result;
}

std::vector<ReflectorState> LoadReflectorsFromConfiguration(const std::string& config_file_content) {
  static const std::string temp_conf_file_name = "temp_configuration_file.txt";

  std::ofstream configuration_file(temp_conf_file_name);
  configuration_file << config_file_content;
  configuration_file.close();

  auto reflectors = LoadReflectorsFromConfigurationFile(temp_conf_file_name);

  std::remove(temp_conf_file_name.c_str());

  return reflectors;
}

bool WakeUp(std::int32_t com_port, ReflectorState& reflector) {
  constexpr static std::size_t kMaxAttempts = 5;

  // clang-format off
  static auto kSendMsgFormat = "R%d s\r\n";
  static auto kReadMsgFormat = "FW v.: %lf - %s";
  // clang-format on

  for (std::size_t attempts = 0; attempts < kMaxAttempts; ++attempts) {
    spdlog::debug("Sending wake up command for reflector: {} - {}", reflector.com_id, reflector.id);
    if (Send(com_port, kSendMsgFormat, reflector.com_id)) {
      double FW = 0.0;
      char useless[200];

      spdlog::debug("Reading after wake up command for reflector: {} - {}", reflector.com_id, reflector.id);
      if (Read(com_port, kReadMsgFormat, &FW, useless) == 2) {
        reflector.firmware = FW;
        return true;
      }
    }
  }
  return false;
}

bool Initialize(std::int32_t com_port, ReflectorState& reflector) {
  // clang-format off
  static auto kSendMsgFormat = "R%d X\r\n";
  static auto kReadMsgFormat = "Mem8[0]: %u, Mem8[1]: %u, VMem32[0]: %u, VMem32[1]: %u, Mem16[0]: %u, Mem16[1]: %u,Mem16[2]: %u, Mem16[3]: %u,Mem16[4]: %u";
  // clang-format on

  spdlog::debug("Sending initialize command for reflector: {} - {}", reflector.com_id, reflector.id);
  if (!Send(com_port, kSendMsgFormat, reflector.com_id)) { throw std::runtime_error("Initialize message sending error."); }

  std::int32_t a8 = 0, b8 = 0, a32 = 0, b32 = 0, a16 = 0, b16 = 0, c16 = 0, d16 = 0, e16 = 0;

  auto start = std::chrono::high_resolution_clock::now();

  std::int32_t read_values = Read(com_port, kReadMsgFormat, &a8, &b8, &a32, &b32, &a16, &b16, &c16, &d16, &e16);
  while (read_values <= 4 && std::chrono::high_resolution_clock::now() - start < kDelayRead) {
    spdlog::debug("Reading after initialize command for reflector: {} - {}", reflector.com_id, reflector.id);
    read_values = Read(com_port, kReadMsgFormat, &a8, &b8, &a32, &b32, &a16, &b16, &c16, &d16, &e16);
  }

  if (read_values <= 4) return false;

  reflector.id_wafer = a32;
  reflector.id_die = b32;

  return true;
}

bool ReadPositioningData(std::int32_t com_port, ReflectorState& reflector) {
  constexpr static std::size_t kMaxAttempts = 10;

  // clang-format off
  static auto kSendMsgFormat = "R%d x %d\r\n";
  static auto kReadMsgFormat = "R%dB%d,ERRAZ%dERREL%d,POS%lf %lf";
  // clang-format on

  for (std::size_t attempts = 0; attempts < kMaxAttempts; ++attempts) {
    spdlog::debug("Sending request positions command for reflector: {} - {}", reflector.com_id, reflector.id);
    if (Send(com_port, kSendMsgFormat, reflector.com_id, reflector.line_num)) {
      double read_pos_az = 0.0, read_pos_el = 0.0;
      std::int32_t read_error = 0, read_error2 = 0, read_mb = 0, read_vb = 0;

      spdlog::debug("Reading after request positions command for reflector: {} - {}", reflector.com_id, reflector.id);
      if (Read(com_port, kReadMsgFormat, &read_vb, &read_mb, &read_error, &read_error2, &read_pos_az, &read_pos_el) == 6) {
        reflector.actual_position_elevation_mm = read_pos_el;
        reflector.actual_position_azimuth_mm = read_pos_az;
        reflector.actual_status_azimuth = read_error;
        reflector.actual_status_elevation = read_error2;
        return true;
      }
    }
  }
  return false;
}

bool SetHall(std::int32_t com_port, ReflectorState& reflector) {
  // clang-format off
  static auto kSendMsgFormat = "R%d H%f\r\n";
  // clang-format on

  spdlog::debug("Sending set hall command for reflector: {} - {}", reflector.com_id, reflector.id);
  return Send(com_port, kSendMsgFormat, reflector.com_id, reflector.hall_spacing);
}

bool Flash(std::int32_t com_port, ReflectorState& reflector) {
  // clang-format off
  static auto kSendMsgFormat = "R%d C\r\n";
  // clang-format on

  spdlog::debug("Sending flash command for reflector: {} - {}", reflector.com_id, reflector.id);
  return Send(com_port, kSendMsgFormat, reflector.com_id);
}

bool Reboot(std::int32_t com_port, ReflectorState& reflector) {
  constexpr static std::size_t kMaxAttempts = 10;

  // clang-format off
  static auto kSendMsgFormat = "R%d P\r\n";
  static auto kReadMsg = "REBOOT";
  // clang-format on

  spdlog::debug("Sending reboot command for reflector: {} - {}", reflector.com_id, reflector.id);

  if (Send(com_port, kSendMsgFormat, reflector.com_id)) {
    std::string out;
    for (std::size_t attempts = 0; attempts < kMaxAttempts; ++attempts) {
      if (Read(com_port, out) && out.find(kReadMsg) != std::string::npos) { return true; }
    }
  }
  return false;
}

bool SetPosition(std::int32_t com_port, ReflectorState& reflector, double azimuth, double elevation) {
  // clang-format off
  static auto kSendAzimuthMsgFormat = "R%d AZ%d%.2f\r\n";
  static auto kSendElevationMsgFormat = "R%d EL%d%.2f\r\n";
  // clang-format on

  spdlog::debug("Sending set position command for reflector: {} - {}", reflector.com_id, reflector.id);
  bool result = Send(com_port, kSendAzimuthMsgFormat, reflector.com_id, reflector.line_num, azimuth);
  result &= Send(com_port, kSendElevationMsgFormat, reflector.com_id, reflector.line_num, elevation);

  return result;
}

bool Move(std::int32_t com_port, ReflectorState& reflector, double azimuth, double elevation) {
  // clang-format off
  static auto kSendStep1MsgFormat = "R%d m\r\n";
  static auto kSendStep2MsgFormat = "R%d %d %+06.2f,%+06.2f\r\n";
  static auto kReadMoveMsg = "Manual adjustment of both motors : +-aa.zz,+-ee.ll mm to specify displacement of azimuth and elevation motors, respectively";
  // clang-format on

  spdlog::debug("Sending go command for reflector: {} - {}", reflector.com_id, reflector.id);
  bool result = Send(com_port, kSendStep1MsgFormat, reflector.com_id);
  std::string out;
  if (Read(com_port, out) && out.find(kReadMoveMsg) == std::string::npos) { return false; }
  result &= Send(com_port, kSendStep2MsgFormat, reflector.com_id, reflector.line_num, azimuth, elevation);

  return result;
}

}  // namespace voltiris::controller::utils