#pragma once

#include <nlohmann/json.hpp>

#include <controller/updates/reflectors_controller_update.hpp>
#include <controller/updates/reflectors_controller_update_visitor.hpp>

namespace voltiris::controller {

class ErrorUpdate : public ReflectorsControllerUpdate {
public:
  inline explicit ErrorUpdate() = default;
  inline explicit ErrorUpdate(std::string error) : _error(std::move(error)) {}
  ~ErrorUpdate() override = default;

  [[nodiscard]] std::string GetError() const { return _error; }

  [[nodiscard]] std::string ToJson() override;

private:
  inline void Visit(ReflectorsControllerUpdateVisitor& visitor) override { visitor.ProcessUpdate(*this); }

  std::string _error;
};

void to_json(nlohmann::json& j, const ErrorUpdate& update);

void from_json(const nlohmann::json& j, ErrorUpdate& update);

}  // namespace voltiris::controller
