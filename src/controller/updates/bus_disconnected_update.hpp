#pragma once

#include <nlohmann/json.hpp>

#include <controller/updates/reflectors_controller_update.hpp>
#include <controller/updates/reflectors_controller_update_visitor.hpp>

namespace voltiris::controller {

class BusDisconnectedUpdate : public ReflectorsControllerUpdate {
public:
  inline explicit BusDisconnectedUpdate() = default;
  ~BusDisconnectedUpdate() override = default;

  [[nodiscard]] std::string ToJson() override;

private:
  inline void Visit(ReflectorsControllerUpdateVisitor& visitor) override { visitor.ProcessUpdate(*this); }
};

void to_json(nlohmann::json& j, const BusDisconnectedUpdate& update);

void from_json(const nlohmann::json& j, BusDisconnectedUpdate& update);

}  // namespace voltiris::controller

