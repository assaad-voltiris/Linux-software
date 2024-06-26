#pragma once

#include <vector>

#include <controller/reflector_state.hpp>

namespace voltiris::controller::utils {

ReflectorState LoadReflectorFromConfiguration(const std::string& config);

std::vector<ReflectorState> LoadReflectorsFromConfigurationFile(const std::string& config_file);

bool WakeUp(std::int32_t com_port, ReflectorState& reflector);

bool Initialize(std::int32_t com_port, ReflectorState& reflector);

bool ReadPositioningData(std::int32_t com_port, ReflectorState& reflector);

bool SetHall(std::int32_t com_port, ReflectorState& reflector);

bool Flash(std::int32_t com_port, ReflectorState& reflector);

bool Reboot(std::int32_t com_port, ReflectorState& reflector);

bool SetPosition(std::int32_t com_port, ReflectorState& reflector, double azimuth, double elevation);

bool Move(std::int32_t com_port, ReflectorState& reflector, double azimuth, double elevation);

}  // namespace voltiris::controller::utils