#pragma once

namespace voltiris::controller {

class ReflectorsControllerCommandVisitor;

class ReflectorsControllerCommand {
public:
  virtual ~ReflectorsControllerCommand() = default;

  virtual void Visit(ReflectorsControllerCommandVisitor&) = 0;
};

}  // namespace voltiris::controller