#pragma once

#include <memory>
#include <string>

namespace voltiris::controller {

class ReflectorsControllerUpdateVisitor;

class ReflectorsControllerUpdate {
public:
  virtual ~ReflectorsControllerUpdate() = default;

  virtual void Visit(ReflectorsControllerUpdateVisitor&) = 0;

  virtual std::string ToJson() = 0;

  static std::unique_ptr<ReflectorsControllerUpdate> FromJson(const std::string& command_json);
};

}  // namespace voltiris::controller
