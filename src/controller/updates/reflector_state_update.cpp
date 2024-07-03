#include "reflector_state_update.hpp"

namespace voltiris::controller {

std::string ReflectorStateUpdate::ToJson() {
  nlohmann::json j = *this;
  return j.dump();
}

void to_json(nlohmann::json& j, const ReflectorStateUpdate& update) {
  const auto& state = update.GetReflectorState();
  j = nlohmann::json{{"id", "ReflectorStateUpdate"},
                     {"reflector_index", update.GetReflectorIndex()},
                     {"reflector_id", state.id},
                     {"com_id", state.com_id},
                     {"line_num", state.line_num},
                     {"latitude", state.latitude},
                     {"longitude", state.longitude},
                     {"hall_spacing", state.hall_spacing},
                     {"firmware", state.firmware},
                     {"a1", state.a1},
                     {"a2", state.a2},
                     {"a3", state.a3},
                     {"id_wafer", state.id_wafer},
                     {"id_die", state.id_die},
                     {"actual_position_elevation_mm", state.actual_position_elevation_mm},
                     {"actual_position_elevation_deg", state.actual_position_elevation_deg},
                     {"actual_position_azimuth_mm", state.actual_position_azimuth_mm},
                     {"actual_position_azimuth_deg", state.actual_position_azimuth_deg},
                     {"actual_position_elevation_previous", state.actual_position_elevation_previous},
                     {"actual_position_azimuth_previous", state.actual_position_azimuth_previous},
                     {"azimuth_is_max", state.azimuth_is_max},
                     {"elevation_is_min", state.elevation_is_min},
                     {"actual_status_azimuth", state.actual_status_azimuth},
                     {"actual_status_elevation", state.actual_status_elevation},
                     {"theoretical_position_elevation_mm", state.theoretical_position_elevation_mm},
                     {"theoretical_position_elevation_deg", state.theoretical_position_elevation_deg},
                     {"theoretical_position_azimuth_mm", state.theoretical_position_azimuth_mm},
                     {"theoretical_position_azimuth_deg", state.theoretical_position_azimuth_deg},
                     {"theoretical_position_elevation_previous", state.theoretical_position_elevation_previous},
                     {"theoretical_position_azimuth_previous", state.theoretical_position_azimuth_previous},
                     {"calibration_motor_azimuth", state.calibration_motor_azimuth},
                     {"calibration_motor_elevation", state.calibration_motor_elevation},
                     {"calibration_elevation_mm", state.calibration_elevation_mm},
                     {"calibration_azimuth_mm", state.calibration_azimuth_mm},
                     {"calibration_elevation_deg", state.calibration_elevation_deg},
                     {"calibration_azimuth_deg", state.calibration_azimuth_deg},
                     {"st_spacing", state.st_spacing}};
}

void from_json(const nlohmann::json& j, ReflectorStateUpdate& update) {
  ReflectorState state;
  state.id = j.at("reflector_id").get<std::int32_t>();
  state.com_id = j.at("com_id").get<std::int32_t>();
  state.line_num = j.at("line_num").get<std::int32_t>();

  state.latitude = j.at("latitude").get<double>();
  state.longitude = j.at("longitude").get<double>();
  state.hall_spacing = j.at("hall_spacing").get<double>();
  state.firmware = j.at("firmware").get<double>();

  state.a1 = j.at("a1").get<double>();
  state.a2 = j.at("a2").get<double>();
  state.a3 = j.at("a3").get<double>();

  state.id_wafer = j.at("id_wafer").get<std::int32_t>();
  state.id_die = j.at("id_die").get<std::int32_t>();

  state.actual_position_elevation_mm = j.at("actual_position_elevation_mm").get<double>();
  state.actual_position_elevation_deg = j.at("actual_position_elevation_deg").get<double>();
  state.actual_position_azimuth_mm = j.at("actual_position_azimuth_mm").get<double>();
  state.actual_position_azimuth_deg = j.at("actual_position_azimuth_deg").get<double>();
  state.actual_position_elevation_previous = j.at("actual_position_elevation_previous").get<double>();
  state.actual_position_azimuth_previous = j.at("actual_position_azimuth_previous").get<double>();

  state.azimuth_is_max = j.at("azimuth_is_max").get<bool>();
  state.elevation_is_min = j.at("elevation_is_min").get<bool>();

  state.actual_status_azimuth = j.at("actual_status_azimuth").get<std::int32_t>();
  state.actual_status_elevation = j.at("actual_status_elevation").get<std::int32_t>();

  state.theoretical_position_elevation_mm = j.at("theoretical_position_elevation_mm").get<double>();
  state.theoretical_position_elevation_deg = j.at("theoretical_position_elevation_deg").get<double>();
  state.theoretical_position_azimuth_mm = j.at("theoretical_position_azimuth_mm").get<double>();
  state.theoretical_position_azimuth_deg = j.at("theoretical_position_azimuth_deg").get<double>();
  state.theoretical_position_elevation_previous = j.at("theoretical_position_elevation_previous").get<double>();
  state.theoretical_position_azimuth_previous = j.at("theoretical_position_azimuth_previous").get<double>();

  state.calibration_motor_azimuth = j.at("calibration_motor_azimuth").get<double>();
  state.calibration_motor_elevation = j.at("calibration_motor_elevation").get<double>();
  state.calibration_elevation_mm = j.at("calibration_elevation_mm").get<double>();
  state.calibration_azimuth_mm = j.at("calibration_azimuth_mm").get<double>();
  state.calibration_elevation_deg = j.at("calibration_elevation_deg").get<double>();
  state.calibration_azimuth_deg = j.at("calibration_azimuth_deg").get<double>();

  state.st_spacing = j.at("st_spacing").get<double>();

  update = ReflectorStateUpdate(j.at("reflector_index").get<std::int32_t>(), state);
}

}  // namespace voltiris::controller