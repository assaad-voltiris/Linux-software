#pragma once

#include <cinttypes>

#include <nlohmann/json.hpp>

#include <controller/updates/reflectors_controller_update.hpp>
#include <controller/updates/reflectors_controller_update_visitor.hpp>

namespace voltiris::controller {

class ASTUpdate : public ReflectorsControllerUpdate {
public:
  inline explicit ASTUpdate() = default;
  inline explicit ASTUpdate(double latitude, double longitude, std::int32_t local_hours, std::int32_t local_minutes, std::int32_t system_hours,
                            std::int32_t system_minutes, double sun_azimuth, double sun_elevation, double sun_ref_azimuth, double sun_ref_elevation, double hra,
                            double hrar)
      : _latitude(latitude),
        _longitude(longitude),
        _local_hours(local_hours),
        _local_minutes(local_minutes),
        _system_hours(system_hours),
        _system_minutes(system_minutes),
        _sun_azimuth(sun_azimuth),
        _sun_elevation(sun_elevation),
        _sun_ref_azimuth(sun_ref_azimuth),
        _sun_ref_elevation(sun_ref_elevation),
        _hra(hra),
        _hrar(hrar) {}
  ~ASTUpdate() override = default;

  [[nodiscard]] double GetLatitude() const { return _latitude; }
  [[nodiscard]] double GetLongitude() const { return _longitude; }
  [[nodiscard]] std::int32_t GetLocalHours() const { return _local_hours; }
  [[nodiscard]] std::int32_t GetLocalMinutes() const { return _local_minutes; }
  [[nodiscard]] std::int32_t GetSystemHours() const { return _system_hours; }
  [[nodiscard]] std::int32_t GetSystemMinutes() const { return _system_minutes; }
  [[nodiscard]] double GetSunAzimuth() const { return _sun_azimuth; }
  [[nodiscard]] double GetSunElevation() const { return _sun_elevation; }
  [[nodiscard]] double GetSunRefAzimuth() const { return _sun_ref_azimuth; }
  [[nodiscard]] double GetSunRefElevation() const { return _sun_ref_elevation; }
  [[nodiscard]] double GetHra() const { return _hra; }
  [[nodiscard]] double GetHrar() const { return _hrar; }

  [[nodiscard]] std::string ToJson() override;

  inline bool operator==(const ASTUpdate& rhs) const {
    return _latitude == rhs._latitude && _longitude == rhs._longitude && _local_hours == rhs._local_hours && _local_minutes == rhs._local_minutes &&
           _system_hours == rhs._system_hours && _system_minutes == rhs._system_minutes && _sun_azimuth == rhs._sun_azimuth &&
           _sun_elevation == rhs._sun_elevation && _sun_ref_azimuth == rhs._sun_ref_azimuth && _sun_ref_elevation == rhs._sun_ref_elevation &&
           _hra == rhs._hra && _hrar == rhs._hrar;
  }

  inline bool operator!=(const ASTUpdate& rhs) const { return !(*this == rhs); }

private:
  inline void Visit(ReflectorsControllerUpdateVisitor& visitor) override { visitor.ProcessUpdate(*this); }

  double _latitude = 0;
  double _longitude = 0;
  std::int32_t _local_hours = 0;
  std::int32_t _local_minutes = 0;
  std::int32_t _system_hours = 0;
  std::int32_t _system_minutes = 0;
  double _sun_azimuth = 0;
  double _sun_elevation = 0;
  double _sun_ref_azimuth = 0;
  double _sun_ref_elevation = 0;
  double _hra = 0;
  double _hrar = 0;
};

void to_json(nlohmann::json& j, const ASTUpdate& update);

void from_json(const nlohmann::json& j, ASTUpdate& update);

}  // namespace voltiris::controller
