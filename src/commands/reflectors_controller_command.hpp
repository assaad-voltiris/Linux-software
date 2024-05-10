#pragma once

namespace voltiris {

class ReflectorsControllerCommandVisitor;

class ReflectorsControllerCommand {
public:
  virtual ~ReflectorsControllerCommand() = default;

  virtual void Visit(ReflectorsControllerCommandVisitor&) = 0;
};

}