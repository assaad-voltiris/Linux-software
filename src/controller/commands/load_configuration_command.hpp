#pragma once

#include <string>

#include <controller/commands/reflectors_controller_command.hpp>
#include <controller/commands/reflectors_controller_command_visitor.hpp>

namespace voltiris::controller {

class LoadConfigurationCommand : public ReflectorsControllerCommand {
public:
  inline explicit LoadConfigurationCommand(const std::string& file_path) : _file_path(file_path) {}
  ~LoadConfigurationCommand() override = default;

  [[nodiscard]] inline const std::string& GetFilePath() const { return _file_path; }

private:
  inline void Visit(ReflectorsControllerCommandVisitor& visitor) override { visitor.ProcessCommand(*this); }

  std::string _file_path;
};

}  // namespace voltiris::controller