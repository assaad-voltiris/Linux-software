#pragma once

#include <string>

#include <nlohmann/json.hpp>

#include <controller/commands/reflectors_controller_command.hpp>
#include <controller/commands/reflectors_controller_command_visitor.hpp>

namespace voltiris::controller {

class LoadConfigurationCommand : public ReflectorsControllerCommand {
public:
  inline explicit LoadConfigurationCommand() = default;
  inline explicit LoadConfigurationCommand(const std::string& file_path, const std::string& file_content = "")
      : _file_path(file_path), _file_content(file_content) {}
  ~LoadConfigurationCommand() override = default;

  [[nodiscard]] inline const std::string& GetFilePath() const { return _file_path; }
  [[nodiscard]] inline const std::string& GetFileContent() const { return _file_content; }

  [[nodiscard]] std::string ToJson() override;

private:
  inline void Visit(ReflectorsControllerCommandVisitor& visitor) override { visitor.ProcessCommand(*this); }

  std::string _file_path;
  std::string _file_content;
};

void to_json(nlohmann::json& j, const LoadConfigurationCommand& command);

void from_json(const nlohmann::json& j, LoadConfigurationCommand& command);

}  // namespace voltiris::controller