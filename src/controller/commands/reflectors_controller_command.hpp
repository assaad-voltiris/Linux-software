#pragma once

#include <string>
#include <memory>

namespace voltiris::controller {

class ReflectorsControllerCommandVisitor;

class ReflectorsControllerCommand {
public:
  virtual ~ReflectorsControllerCommand() = default;

  virtual void Visit(ReflectorsControllerCommandVisitor&) = 0;

  virtual std::string ToJson() = 0;

  static std::unique_ptr<ReflectorsControllerCommand> FromJson(const std::string& command_json);
};

}  // namespace voltiris::controller