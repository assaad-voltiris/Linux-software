#pragma once

#include <string>

#include <controller/commands/reflectors_controller_command.hpp>
#include <controller/commands/reflectors_controller_command_visitor.hpp>

namespace voltiris::controller {

class ConnectCommand : public ReflectorsControllerCommand {
public:
  inline explicit ConnectCommand(const std::string& bus_name) : _bus_name(bus_name) {}
  ~ConnectCommand() override = default;

  [[nodiscard]] const std::string& GetBusName() const { return _bus_name; }

private:
  inline void Visit(ReflectorsControllerCommandVisitor& visitor) override { visitor.ProcessCommand(*this); }

  std::string _bus_name;
};

}  // namespace voltiris::controller