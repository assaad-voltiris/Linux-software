#pragma once

#include <string>

#include <nlohmann/json.hpp>

#include <controller/commands/reflectors_controller_command.hpp>
#include <controller/commands/reflectors_controller_command_visitor.hpp>

namespace voltiris::controller {

class ConnectCommand : public ReflectorsControllerCommand {
public:
  inline explicit ConnectCommand() = default;
  inline explicit ConnectCommand(const std::string& bus_name) : _bus_name(bus_name) {}
  ~ConnectCommand() override = default;

  [[nodiscard]] const std::string& GetBusName() const { return _bus_name; }

  [[nodiscard]] std::string ToJson() override;

private:
  inline void Visit(ReflectorsControllerCommandVisitor& visitor) override { visitor.ProcessCommand(*this); }

  std::string _bus_name;
};

void to_json(nlohmann::json& j, const ConnectCommand& command);

void from_json(const nlohmann::json& j, ConnectCommand& command);

}  // namespace voltiris::controller