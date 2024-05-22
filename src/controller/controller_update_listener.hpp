#pragma once

#include <cstdint>
#include <string>

#include <controller/controller_status.hpp>
#include <controller/reflector_state.hpp>

namespace voltiris::controller {

class ControllerUpdateListener {
public:
  // General
  virtual void OnStatusUpdate(ControllerStatus new_status) = 0;
  virtual void OnError(const std::string& error_string) = 0;

  // Connection
  virtual void OnBusListUpdate(const std::vector<std::string>& available_buss) = 0;
  virtual void OnBusConnected() = 0;
  virtual void OnBusDisconnected() = 0;

  //
  virtual void OnASTLatitude(double value) = 0;
  virtual void OnASTLongitude(double value) = 0;
  virtual void OnASTLocalTime(std::int32_t hours, std::int32_t minutes) = 0;
  virtual void OnASTSystemTime(std::int32_t hours, std::int32_t minutes) = 0;
  virtual void OnASTSunAzimuth(double value) = 0;
  virtual void OnASTSunElevation(double value) = 0;
  virtual void OnASTSunRefAzimuth(double value) = 0;
  virtual void OnASTSunRefElevation(double value) = 0;
  virtual void OnASTHra(double value) = 0;
  virtual void OnASTHrar(double value) = 0;

  //
  virtual void OnCPAzimuthMotorPosition(double value) = 0;
  virtual void OnCPAzimuthAngle(double value) = 0;
  virtual void OnCPElevationMotorPosition(double value) = 0;
  virtual void OnCPElevationAngle(double value) = 0;

  //
  virtual void OnRDReflectorsSize(std::size_t size) = 0;
  virtual void OnRDReflectorState(std::size_t reflector_index, const ReflectorState& new_state) = 0;
};

}  // namespace voltiris::controller