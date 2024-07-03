#pragma once

#include <cinttypes>

#include <nlohmann/json.hpp>

#include <controller/reflector_state.hpp>
#include <controller/updates/reflectors_controller_update.hpp>
#include <controller/updates/reflectors_controller_update_visitor.hpp>

namespace voltiris::controller {

class ReflectorStateUpdate : public ReflectorsControllerUpdate {
public:
  inline explicit ReflectorStateUpdate() = default;
  inline explicit ReflectorStateUpdate(std::size_t reflector_index, const ReflectorState& state) : _reflector_index(reflector_index), _state(state) {}
  ~ReflectorStateUpdate() override = default;

  [[nodiscard]] std::size_t GetReflectorIndex() const { return _reflector_index; }
  [[nodiscard]] const ReflectorState& GetReflectorState() const { return _state; }

  [[nodiscard]] std::string ToJson() override;

private:
  inline void Visit(ReflectorsControllerUpdateVisitor& visitor) override { visitor.ProcessUpdate(*this); }

  std::size_t _reflector_index = 0;
  ReflectorState _state;
};

void to_json(nlohmann::json& j, const ReflectorStateUpdate& update);

void from_json(const nlohmann::json& j, ReflectorStateUpdate& update);

}  // namespace voltiris::controller
