#pragma once

#include <string>

namespace voltiris {

class ReflectorsDataObserver {
public:
  virtual void OnError(const std::string& error_string) = 0;

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

  virtual void OnCPAzimuthMotorPosition(double value) = 0;
  virtual void OnCPAzimuthAngle(double value) = 0;
  virtual void OnCPElevationMotorPosition(double value) = 0;
  virtual void OnCPElevationAngle(double value) = 0;

  virtual void OnRDReflectorPositionAzimuthDeg(std::size_t reflector_id, double value) = 0;
  virtual void OnRDReflectorPositionElevationDeg(std::size_t reflector_id, double value) = 0;
  virtual void OnRDReflectorPositionAzimuthMm(std::size_t reflector_id, double value) = 0;
  virtual void OnRDReflectorPositionElevationMm(std::size_t reflector_id, double value) = 0;
  virtual void OnRDReflectorStatusAzimuth(std::size_t reflector_id, double value) = 0;
  virtual void OnRDReflectorStatusElevation(std::size_t reflector_id, double value) = 0;
  virtual void OnRDReflectorInfo(std::size_t reflector_id, double value) = 0;
  virtual void OnRDReflectorThPositionAzimuthDeg(std::size_t reflector_id, double value) = 0;
  virtual void OnRDReflectorThPositionElevationDeg(std::size_t reflector_id, double value) = 0;
  virtual void OnRDReflectorThPositionAzimuthMm(std::size_t reflector_id, double value) = 0;
  virtual void OnRDReflectorThPositionElevationMm(std::size_t reflector_id, double value) = 0;
};

}