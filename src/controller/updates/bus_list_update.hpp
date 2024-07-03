#pragma once

#include <utility>

#include <nlohmann/json.hpp>

#include <controller/updates/reflectors_controller_update.hpp>
#include <controller/updates/reflectors_controller_update_visitor.hpp>

namespace voltiris::controller {

class BusListUpdate : public ReflectorsControllerUpdate {
public:
  inline explicit BusListUpdate() = default;
  inline explicit BusListUpdate(std::vector<std::string> available_buss) : _available_buss(std::move(available_buss)) {}
  ~BusListUpdate() override = default;

  [[nodiscard]] const std::vector<std::string>& GetAvailableBuss() const { return _available_buss; }

  [[nodiscard]] std::string ToJson() override;

private:
  inline void Visit(ReflectorsControllerUpdateVisitor& visitor) override { visitor.ProcessUpdate(*this); }

  std::vector<std::string> _available_buss;
};

void to_json(nlohmann::json& j, const BusListUpdate& update);

void from_json(const nlohmann::json& j, BusListUpdate& update);

}  // namespace voltiris::controller
