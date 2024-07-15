#include "reflector_state_update.hpp"

namespace voltiris::controller {

std::string ReflectorStateUpdate::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const ReflectorStateUpdate& update) {
  const auto& state = update.GetReflectorState();
  j = nlohmann::json{{"id", "ReflectorState"},
                     {"i", update.GetReflectorIndex()},
                     {"r", state.id},
                     {"c", state.com_id},
                     {"l", state.line_num},
                     {"la", state.latitude},
                     {"lo", state.longitude},
                     {"h", state.hall_spacing},
                     {"f", state.firmware},
                     {"a1", state.a1},
                     {"a2", state.a2},
                     {"a3", state.a3},
                     {"w", state.id_wafer},
                     {"d", state.id_die},
                     {"ae_mm", state.actual_position_elevation_mm},
                     {"ae_deg", state.actual_position_elevation_deg},
                     {"aa_mm", state.actual_position_azimuth_mm},
                     {"aa_deg", state.actual_position_azimuth_deg},
                     {"aep", state.actual_position_elevation_previous},
                     {"aap", state.actual_position_azimuth_previous},
                     {"a_is_max", state.azimuth_is_max},
                     {"e_is_min", state.elevation_is_min},
                     {"sa", state.actual_status_azimuth},
                     {"se", state.actual_status_elevation},
                     {"the_mm", state.theoretical_position_elevation_mm},
                     {"the_deg", state.theoretical_position_elevation_deg},
                     {"tha_mm", state.theoretical_position_azimuth_mm},
                     {"tha_deg", state.theoretical_position_azimuth_deg},
                     {"thep", state.theoretical_position_elevation_previous},
                     {"thap", state.theoretical_position_azimuth_previous},
                     {"cma", state.calibration_motor_azimuth},
                     {"cme", state.calibration_motor_elevation},
                     {"ce_mm", state.calibration_elevation_mm},
                     {"ca_mm", state.calibration_azimuth_mm},
                     {"ce_deg", state.calibration_elevation_deg},
                     {"ca_deg", state.calibration_azimuth_deg},
                     {"sp", state.st_spacing}};
}

void from_json(const nlohmann::json& j, ReflectorStateUpdate& update) {
  ReflectorState state;
  state.id = j.at("r").get<std::int32_t>();
  state.com_id = j.at("c").get<std::int32_t>();
  state.line_num = j.at("l").get<std::int32_t>();

  state.latitude = j.at("la").get<double>();
  state.longitude = j.at("lo").get<double>();
  state.hall_spacing = j.at("h").get<double>();
  state.firmware = j.at("f").get<double>();

  state.a1 = j.at("a1").get<double>();
  state.a2 = j.at("a2").get<double>();
  state.a3 = j.at("a3").get<double>();

  state.id_wafer = j.at("w").get<std::int32_t>();
  state.id_die = j.at("d").get<std::int32_t>();

  state.actual_position_elevation_mm = j.at("ae_mm").get<double>();
  state.actual_position_elevation_deg = j.at("ae_deg").get<double>();
  state.actual_position_azimuth_mm = j.at("aa_mm").get<double>();
  state.actual_position_azimuth_deg = j.at("aa_deg").get<double>();
  state.actual_position_elevation_previous = j.at("aep").get<double>();
  state.actual_position_azimuth_previous = j.at("aap").get<double>();

  state.azimuth_is_max = j.at("a_is_max").get<bool>();
  state.elevation_is_min = j.at("e_is_min").get<bool>();

  state.actual_status_azimuth = j.at("sa").get<std::int32_t>();
  state.actual_status_elevation = j.at("se").get<std::int32_t>();

  state.theoretical_position_elevation_mm = j.at("the_mm").get<double>();
  state.theoretical_position_elevation_deg = j.at("the_deg").get<double>();
  state.theoretical_position_azimuth_mm = j.at("tha_mm").get<double>();
  state.theoretical_position_azimuth_deg = j.at("tha_deg").get<double>();
  state.theoretical_position_elevation_previous = j.at("thep").get<double>();
  state.theoretical_position_azimuth_previous = j.at("thap").get<double>();

  state.calibration_motor_azimuth = j.at("cma").get<double>();
  state.calibration_motor_elevation = j.at("cme").get<double>();
  state.calibration_elevation_mm = j.at("ce_mm").get<double>();
  state.calibration_azimuth_mm = j.at("ca_mm").get<double>();
  state.calibration_elevation_deg = j.at("ce_deg").get<double>();
  state.calibration_azimuth_deg = j.at("ca_deg").get<double>();

  state.st_spacing = j.at("sp").get<double>();

  update = ReflectorStateUpdate(j.at("i").get<std::size_t>(), state);
}

}  // namespace voltiris::controller