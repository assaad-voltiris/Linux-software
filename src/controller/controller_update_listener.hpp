#pragma once

#include <cstdint>
#include <string>

#include <controller/controller_status.hpp>
#include <controller/reflector_state.hpp>
#include <controller/updates/reflectors_controller_update.hpp>

namespace voltiris::controller {

class ControllerUpdateListener {
public:
  virtual void OnUpdate(std::unique_ptr<ReflectorsControllerUpdate> update) = 0;
};

}  // namespace voltiris::controller