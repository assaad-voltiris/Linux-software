#pragma once

#include <cinttypes>

#include <nlohmann/json.hpp>

#include <controller/updates/reflectors_controller_update.hpp>
#include <controller/updates/reflectors_controller_update_visitor.hpp>

namespace voltiris::controller {

class CurrentConfigurationUpdate : public ReflectorsControllerUpdate {
public:
  inline explicit CurrentConfigurationUpdate() = default;
  inline explicit CurrentConfigurationUpdate(double latitude, double longitude, double cycle_frequency, bool starting_hra_enabled, double starting_hra,
                                             double acceleration_factor)
      : _latitude(latitude),
        _longitude(longitude),
        _cycle_frequency(cycle_frequency),
        _starting_hra_enabled(starting_hra_enabled),
        _starting_hra(starting_hra),
        _acceleration_factor(acceleration_factor) {}
  ~CurrentConfigurationUpdate() override = default;

  [[nodiscard]] double GetLatitude() const { return _latitude; }
  [[nodiscard]] double GetLongitude() const { return _longitude; }
  [[nodiscard]] double GetCycleFrequency() const { return _cycle_frequency; }
  [[nodiscard]] bool GetStartingHraEnabled() const { return _starting_hra_enabled; }
  [[nodiscard]] double GetStartingHra() const { return _starting_hra; }
  [[nodiscard]] double GetAccelerationFactor() const { return _acceleration_factor; }

  [[nodiscard]] std::string ToJson() override;

private:
  inline void Visit(ReflectorsControllerUpdateVisitor& visitor) override { visitor.ProcessUpdate(*this); }

  double _latitude = 0;
  double _longitude = 0;
  double _cycle_frequency = 0;
  bool _starting_hra_enabled = false;
  double _starting_hra = 0;
  double _acceleration_factor = 0;
};

void to_json(nlohmann::json& j, const CurrentConfigurationUpdate& update);

void from_json(const nlohmann::json& j, CurrentConfigurationUpdate& update);

}  // namespace voltiris::controller
