#pragma once

#include <imgui.h>

#include <array>
#include <cstdint>
#include <reflectors_commands_observer.hpp>
#include <reflectors_data_observer.hpp>
#include <resources.hpp>
#include <string>
#include <vector>

namespace voltiris {

class STWindow : public ReflectorsDataObserver {
 public:
  explicit STWindow(ReflectorsCommandsObserver& commands_observer);

  void Render(float scale);

 private:
  // ReflectorsDataObserver interface
  void OnError(const std::string& error_string) override;

  void OnASTLatitude(double value) override { _ast_latitude = value; }
  void OnASTLongitude(double value) override { _ast_longitude = value; }
  void OnASTLocalTime(std::int32_t hours, std::int32_t minutes) override {
    _ast_local_time_h = hours;
    _ast_local_time_m = minutes;
  }
  void OnASTSystemTime(std::int32_t hours, std::int32_t minutes) override {
    _ast_system_time_h = hours;
    _ast_system_time_m = minutes;
  }
  void OnASTSunAzimuth(double value) override { _ast_sun_azimuth = value; }
  void OnASTSunElevation(double value) override { _ast_sun_elevation = value; }
  void OnASTSunRefAzimuth(double value) override { _ast_sun_ref_azimuth = value; }
  void OnASTSunRefElevation(double value) override { _ast_sun_ref_elevation = value; }
  void OnASTHra(double value) override { _ast_hra = value; }
  void OnASTHrar(double value) override { _ast_hrar = value; }

  void OnCPAzimuthMotorPosition(double value) override { _cp_azimuth_motor_pos = value; }
  void OnCPAzimuthAngle(double value) override { _cp_azimuth_angle = value; }
  void OnCPElevationMotorPosition(double value) override { _cp_elevation_motor_pos = value; }
  void OnCPElevationAngle(double value) override { _cp_elevation_angle = value; }

  void OnRDReflectorPositionAzimuthDeg(std::size_t reflector_id, double value) override { _rd_data[reflector_id].pos_az_deg = value; };
  void OnRDReflectorPositionAzimuthMm(std::size_t reflector_id, double value) override { _rd_data[reflector_id].pos_az_mm = value; };
  void OnRDReflectorPositionElevationDeg(std::size_t reflector_id, double value) override { _rd_data[reflector_id].pos_el_deg = value; }
  void OnRDReflectorPositionElevationMm(std::size_t reflector_id, double value) override { _rd_data[reflector_id].pos_el_mm = value; }
  void OnRDReflectorStatusAzimuth(std::size_t reflector_id, double value) override { _rd_data[reflector_id].status_az = value; }
  void OnRDReflectorStatusElevation(std::size_t reflector_id, double value) override { _rd_data[reflector_id].status_el = value; }
  void OnRDReflectorInfo(std::size_t reflector_id, double value) override { _rd_data[reflector_id].reflector_info = value; }
  void OnRDReflectorThPositionAzimuthDeg(std::size_t reflector_id, double value) override { _rd_data[reflector_id].th_r_pos_az_deg = value; }
  void OnRDReflectorThPositionAzimuthMm(std::size_t reflector_id, double value) override { _rd_data[reflector_id].th_r_pos_az_mm = value; }
  void OnRDReflectorThPositionElevationDeg(std::size_t reflector_id, double value) override { _rd_data[reflector_id].th_r_pos_el_deg = value; }
  void OnRDReflectorThPositionElevationMm(std::size_t reflector_id, double value) override { _rd_data[reflector_id].th_r_pos_el_mm = value; }

 private:
  // Components rendering
  void RenderMenu(float scale);
  void RenderReflectorsConfiguration(float scale);
  void RenderAcceleratedTrackingSettings(float scale);
  void RenderManualReflectorsAlignment(float scale);
  void RenderAutomaticSunTracking(float scale);
  void RenderCalibrationPosition(float scale);
  void RenderReflectorsData(float scale);

  void RenderError(float scale);

 private:
  ReflectorsCommandsObserver& _commands_observer;

  bool _error_popup_opened = false;
  std::string _current_error;

  // Menu
  bool _menu_file_new = false;
  bool _menu_file_open = false;
  bool _menu_config_reflectors_configuration = false;
  bool _menu_log_window = false;

  // ReflectorsConfiguration
  bool _rc_opened = false;
  char _rc_logs_dir[256] = "";
  int _rc_address_selected_i[6] = {0, 0, 0, 0, 0, 0};
  bool _rc_connected[6] = {false, false, false, false, false, false};
  int _rc_comm_id_selected_i[6] = {0, 0, 0, 0, 0, 0};

  // AcceleratedTrackingSettings
  std::int32_t _atc_cycle_frequency_selected_i = Resources::kATSCycleFrequencyDefaultIndex;

  bool _atc_checkbox_400_0 = Resources::k400m0DefaultChecked;
  bool _atc_checkbox_to_th_pos = Resources::kToThPosDefaultChecked;

  bool _atc_checkbox_90_deg = false;
  bool _atc_checkbox_90_deg_enabled = false;

  bool _atc_checkbox_init_pos = false;
  bool _atc_checkbox_init_pos_enabled = false;

  bool _atc_starting_hra_enabled = false;
  double _atc_starting_hra = Resources::kATSStartingHraDefaultValue;

  std::int32_t _atc_acceleration_factor_i = Resources::kATSAccelerationFactorDefaultIndex;

  // ManualReflectorsAlignment
  double _mra_step_size = Resources::kMRAStepSizeDefaultValue;
  char _mra_reflectors_to_move[128] = "";

  // AutomaticSunTracking
  double _ast_latitude = Resources::kASTLatitudeDefaultValue;
  double _ast_longitude = Resources::kASTLongitudeDefaultValue;
  std::int32_t _ast_count_cycles = Resources::kASTCountCyclesDefaultValue;

  std::int32_t _ast_local_time_h = 0;
  std::int32_t _ast_local_time_m = 0;
  std::int32_t _ast_system_time_h = 0;
  std::int32_t _ast_system_time_m = 0;
  double _ast_sun_azimuth = 0;
  double _ast_sun_elevation = 0;
  double _ast_sun_ref_azimuth = 0;
  double _ast_sun_ref_elevation = 0;
  double _ast_hra = 0;
  double _ast_hrar = 0;

  // CalibrationPosition
  double _cp_azimuth_motor_pos = 0;
  double _cp_azimuth_angle = 0;
  double _cp_elevation_motor_pos = 0;
  double _cp_elevation_angle = 0;

  // ReflectorsData
  bool _rd_is_deg = false;

  struct ReflectorData {
    double pos_az_deg = 0;
    double pos_az_mm = 0;
    double status_az = 0;
    double pos_el_deg = 0;
    double pos_el_mm = 0;
    double status_el = 0;
    double reflector_info = 0;
    double th_r_pos_az_deg = 0;
    double th_r_pos_az_mm = 0;
    double th_r_pos_el_deg = 0;
    double th_r_pos_el_mm = 0;
    char pos_az_ctrl[32] = "";
    char pos_el_ctrl[32] = "";
  };
  std::array<ReflectorData, 16> _rd_data;
};

}  // namespace voltiris
