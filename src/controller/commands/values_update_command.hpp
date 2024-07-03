#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include <controller/commands/reflectors_controller_command.hpp>
#include <controller/commands/reflectors_controller_command_visitor.hpp>

namespace voltiris::controller {

class ValuesUpdateCommand : public ReflectorsControllerCommand {
public:
  enum class ValueId { kAccelerationFactor, kStartingHraEnabled, kStartingHra, kCycleFrequency, kLatitude, kLongitude };

  struct ValueUpdate {
    ValueUpdate(ValueId in_value_id, double in_value) : value_id(in_value_id), value_double(in_value) {}

    ValueUpdate(ValueId in_value_id, std::size_t in_value) : value_id(in_value_id), value_uint(in_value) {}

    ValueUpdate(ValueId in_value_id, bool in_value) : value_id(in_value_id), value_bool(in_value) {}

    ValueId value_id;
    union {
      double value_double;
      std::size_t value_uint;
      bool value_bool;
    };
  };

  inline explicit ValuesUpdateCommand() = default;
  inline explicit ValuesUpdateCommand(const std::vector<ValueUpdate>& updates) : _updates(updates) {}
  ~ValuesUpdateCommand() override = default;

  inline const std::vector<ValueUpdate>& GetUpdates() const { return _updates; }

  [[nodiscard]] std::string ToJson() override;

private:
  inline void Visit(ReflectorsControllerCommandVisitor& visitor) override { visitor.ProcessCommand(*this); }

  std::vector<ValueUpdate> _updates;
};

void to_json(nlohmann::json& j, const ValuesUpdateCommand& command);

void from_json(const nlohmann::json& j, ValuesUpdateCommand& command);

}  // namespace voltiris::controller