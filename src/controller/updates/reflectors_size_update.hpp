#pragma once

#include <nlohmann/json.hpp>

#include <controller/updates/reflectors_controller_update.hpp>
#include <controller/updates/reflectors_controller_update_visitor.hpp>

namespace voltiris::controller {

class ReflectorsSizeUpdate : public ReflectorsControllerUpdate {
public:
  inline explicit ReflectorsSizeUpdate() = default;
  inline explicit ReflectorsSizeUpdate(std::size_t size) : _size(size) {}
  ~ReflectorsSizeUpdate() override = default;

  [[nodiscard]] std::size_t GetSize() const { return _size; }

  [[nodiscard]] std::string ToJson() override;

private:
  inline void Visit(ReflectorsControllerUpdateVisitor& visitor) override { visitor.ProcessUpdate(*this); }

  std::size_t _size = 0;
};

void to_json(nlohmann::json& j, const ReflectorsSizeUpdate& update);

void from_json(const nlohmann::json& j, ReflectorsSizeUpdate& update);

}  // namespace voltiris::controller
