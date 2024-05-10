#pragma once

#include <memory>
#include <string>

namespace voltiris {

class ArduinoMega {
public:
//  explicit ArduinoMega();
//  explicit ArduinoMega(char* com_port, /*DWORD COM_BAUD_RATE*/);
//  ~ArduinoMega() = default;

  double ConvertAZ_Mm2Deg(double mm_value) const;
  double ConvertAZ_Deg2Mm(double deg_value) const;
  double ConvertEL_Mm2Deg(double mm_value) const; //each line of the config file provides its own parameters of conversion
  double ConvertEL_Deg2Mm(double deg_value) const;
  double ConvertEL_Mm2DegTh(double mm_value) const;

//  void addLineToCSVLog();
//  void openOrCreateCSVLog(std::string num);

//  const std::string uC_OK="DONE";
//  const double kA1= 0.8185;
//  const double kA2= 0.23386;
//  const double kA3= 0.36378;
//  double position_azimuth;
//  double position_elevation;
//  int status;
//  double motor_azimuth_mm;
//  double motor_elevation_mm;
  const double kMaxSECU = 39.0;
  double azimuth_is_max = false;
  double elevation_is_min = false;

//  const int kReplyWaitTime = 30;
//  int ReflectorStor;
  double reflector_actual_position_elevation; //Current position EL(mm) of reflector
  double reflector_actual_position_azimuth; //Current position AZ(mm) of reflector
  int reflector_actual_status_azimuth; //Current error status of reflector
  int reflector_actual_status_elevation; //Current error status of reflector
//  int Nstep; //Current error status of reflector
  double reflector_th_position_elevation; //Current theoretical position EL(mm) of reflector
  double reflector_th_position_azimuth; //Current position AZ(mm) of reflector
  double reflector_th_position_elevation_old; //Previous theoretical position EL(mm) of reflector
  double reflector_th_position_azimuth_old; //Previous theoretical position EL(mm) of reflector
  double reflector_calibration_motor_azimuth; //Calibration position of reflector when 0,400 : morning position
  double reflector_calibration_motor_elevation; ////Calibration position of reflector when 0,400 : morning position
  double reflector_a1; //Mechanical factors a1 of reflector
  double reflector_a2; //Mechanical factors  a2 of reflector
  double reflector_a3; //Mechanical factors  a2 of reflector
  double reflector_calibration_elevation_mm; //Calibration position EL(mm) of reflector
  double reflector_calibration_azimuth_mm; //Calibration position AZ(mm) of reflector
  double reflector_calibration_elevation_deg; //Calibration position EL(degree) of reflector
  double reflector_calibration_azimuth_deg; //Calibration position AZ(degree) of reflector
  double reflector_latitude; //Latitude
  double reflector_longitude; //Longitude
  double reflector_hall_spacing; //Hall spacing mm
  double reflector_firmware; //Firmware
  int reflector_id_wafer; //unique ID1
  int reflector_id_die; //unique ID2
  double reflector_actual_position_elevation_old; //Old position EL(mm) of reflector--if reboot
  double reflector_actual_position_azimuth_old; //Old position AZ(mm) of reflector--if reboot
  int ref_id;
  int com_id;
  int line_num;
  double st_spacing =5.0;


private:

};

} // namespace voltiris