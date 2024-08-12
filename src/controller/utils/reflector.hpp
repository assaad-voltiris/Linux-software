#pragma once

#include <vector>

#include <controller/reflector_state.hpp>

namespace voltiris::controller::utils {

ReflectorState LoadReflectorFromConfiguration(const std::string& config);

std::vector<ReflectorState> LoadReflectorsFromConfigurationFile(const std::string& config_file);

std::vector<ReflectorState> LoadReflectorsFromConfiguration(const std::string& config_file_content);

bool WakeUp(std::int32_t com_port, ReflectorState& reflector);

bool Initialize(std::int32_t com_port, ReflectorState& reflector);

bool ReadPositioningData(std::int32_t com_port, ReflectorState& reflector);

bool SetHall(std::int32_t com_port, ReflectorState& reflector);

bool Flash(std::int32_t com_port, ReflectorState& reflector);

bool Reboot(std::int32_t com_port, ReflectorState& reflector);

bool SetPosition(std::int32_t com_port, ReflectorState& reflector, double azimuth, double elevation);

bool Move(std::int32_t com_port, ReflectorState& reflector, double azimuth, double elevation);

bool MoveTo(std::int32_t com_port, ReflectorState &reflector, double target_azimuth, double target_elevation);

bool StepMoveTo(std::int32_t com_port, ReflectorState &reflector, double target_azimuth, double target_elevation);

bool MoveOn(std::int32_t com_port, ReflectorState &reflector, double delta_azimuth, double delta_elevation);

bool StepMoveOn(std::int32_t com_port, ReflectorState &reflector, double delta_azimuth, double delta_elevation);

}  // namespace voltiris::controller::utils