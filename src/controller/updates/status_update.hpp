#pragma once

#include <nlohmann/json.hpp>

#include <controller/controller_status.hpp>
#include <controller/updates/reflectors_controller_update.hpp>
#include <controller/updates/reflectors_controller_update_visitor.hpp>

namespace voltiris::controller {

class StatusUpdate : public ReflectorsControllerUpdate {
public:
  inline explicit StatusUpdate() = default;
  inline explicit StatusUpdate(ControllerStatus status) : _status(status) {}
  ~StatusUpdate() override = default;

  [[nodiscard]] ControllerStatus GetStatus() const { return _status; }

  [[nodiscard]] std::string ToJson() override;

private:
  inline void Visit(ReflectorsControllerUpdateVisitor& visitor) override { visitor.ProcessUpdate(*this); }

  ControllerStatus _status = ControllerStatus::kIdle;
};

void to_json(nlohmann::json& j, const StatusUpdate& update);

void from_json(const nlohmann::json& j, StatusUpdate& update);

}  // namespace voltiris::controller
