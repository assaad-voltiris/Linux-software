#pragma once

#include <string>

#include <commands/reflectors_controller_command.hpp>
#include <commands/reflectors_controller_command_visitor.hpp>

namespace voltiris {

class LoadConfigurationCommand : public ReflectorsControllerCommand {
public:
  inline explicit LoadConfigurationCommand(std::size_t bus_index,
                                           std::string com_id,
                                           const std::string file_path)
      : _bus_index(bus_index), _com_id(com_id), _file_path(file_path) {}
  ~LoadConfigurationCommand() override = default;

  inline std::size_t GetBusIndex() const { return _bus_index; }
  inline std::string GetComId() const { return _com_id; }
  inline std::string GetFilePath() const { return _file_path; }

private:
  inline void Visit(ReflectorsControllerCommandVisitor &visitor) override {
    visitor.ProcessCommand(*this);
  }

  std::size_t _bus_index;
  std::string _com_id;
  std::string _file_path;
};

} // namespace voltiris