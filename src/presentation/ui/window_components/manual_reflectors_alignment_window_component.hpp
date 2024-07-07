//
// Created by sergi on 5/22/2024.
//

#pragma once

#include <string>
#include <vector>

#include <controller/controller_commands_handler.hpp>
#include <controller/reflector_state.hpp>

#include <presentation/ui/resources.hpp>
#include <presentation/ui/window_components/window_component.hpp>

namespace voltiris::presentation::ui {

class ManualReflectorsAlignmentWindowComponent : public WindowComponent {
public:
  explicit ManualReflectorsAlignmentWindowComponent(controller::ControllerCommandsHandler& commands_handler) : WindowComponent(commands_handler) {}

  void Render(double scale) override;

  // RENDERING LOGIC

  // DATA
  void OnReflectorsSize(std::size_t size) {
    _reflectors_data.clear();
    _reflectors_data.resize(size);
  };
  void OnReflectorState(std::size_t reflector_index, const controller::ReflectorState& new_state) { _reflectors_data[reflector_index].id = new_state.id; }

private:
  // RENDERING LOGIC
  double _step_size = Resources::kMRAStepSizeDefaultValue;
  char _reflectors_to_move[128] = "";

  // DATA
  struct ReflectorData {
    std::int32_t id;
    bool to_move;
  };
  std::vector<ReflectorData> _reflectors_data;

  static std::vector<std::size_t> GetReflectorsToMove(const std::vector<ReflectorData>& reflectors_data);
};

}  // namespace voltiris::presentation::ui
