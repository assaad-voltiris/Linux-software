//
// Created by sergi on 5/22/2024.
//

#pragma once

#include <string>

#include <controller/controller_commands_handler.hpp>

#include <presentation/ui/resources.hpp>
#include <presentation/ui/window_components/window_component.hpp>

namespace voltiris::presentation::ui {

class ManualReflectorsAlignmentWindowComponent : public WindowComponent {
public:
  explicit ManualReflectorsAlignmentWindowComponent(controller::ControllerCommandsHandler& commands_handler) : WindowComponent(commands_handler) {}

  void Render(double scale) override;

  // RENDERING LOGIC

  // DATA

private:
  // RENDERING LOGIC
  double _step_size = Resources::kMRAStepSizeDefaultValue;
  char _reflectors_to_move[128] = "";

  // DATA
};

}  // namespace voltiris::presentation::ui
