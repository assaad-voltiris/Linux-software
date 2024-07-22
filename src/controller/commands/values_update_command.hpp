#pragma once

#include <optional>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include <controller/commands/reflectors_controller_command.hpp>
#include <controller/commands/reflectors_controller_command_visitor.hpp>

namespace voltiris::controller {

class ValuesUpdateCommand : public ReflectorsControllerCommand {
public:
  inline explicit ValuesUpdateCommand() = default;
  ~ValuesUpdateCommand() override = default;

  inline void SetStartingHra(double value) { _starting_hra = value; }
  inline void SetAccelerationFactor(std::int32_t value) { _acceleration_factor = value; }

  [[nodiscard]] inline const std::optional<double>& GetStartingHra() const { return _starting_hra; }
  [[nodiscard]] inline const std::optional<std::int32_t>& GetAccelerationFactor() const { return _acceleration_factor; }

  [[nodiscard]] std::string ToJson() override;

private:
  inline void Visit(ReflectorsControllerCommandVisitor& visitor) override { visitor.ProcessCommand(*this); }

  std::optional<double> _starting_hra;
  std::optional<std::int32_t> _acceleration_factor;
};

void to_json(nlohmann::json& j, const ValuesUpdateCommand& command);

void from_json(const nlohmann::json& j, ValuesUpdateCommand& command);

}  // namespace voltiris::controller