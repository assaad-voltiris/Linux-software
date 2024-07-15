//
// Created by sergi on 5/22/2024.
//

#include "manual_reflectors_alignment_window_component.hpp"

#include <imgui.h>

#include <controller/commands/manual_move_command.hpp>

namespace voltiris::presentation::ui {

void ManualReflectorsAlignmentWindowComponent::Render(double scale) {  // General group for AcceleratedTrackingSettings
  ImGui::BeginGroup();

  ImGui::BeginChild("Manual Reflectors Alignment", {}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle);
  ImGui::SeparatorText("Manual Reflectors Alignment");

  if (ImGui::Button("UP")) {
    SendCommand(
        std::make_unique<controller::ManualMoveCommand>(controller::ManualMoveCommand::Direction::kUp, _step_size, GetReflectorsToMove(_reflectors_data)));
  }
  ImGui::SameLine();
  if (ImGui::Button("DOWN")) {
    SendCommand(
        std::make_unique<controller::ManualMoveCommand>(controller::ManualMoveCommand::Direction::kDown, _step_size, GetReflectorsToMove(_reflectors_data)));
  }
  ImGui::SameLine();
  if (ImGui::Button("EAST")) {
    SendCommand(
        std::make_unique<controller::ManualMoveCommand>(controller::ManualMoveCommand::Direction::kEast, _step_size, GetReflectorsToMove(_reflectors_data)));
  }
  ImGui::SameLine();
  if (ImGui::Button("WEST")) {
    SendCommand(
        std::make_unique<controller::ManualMoveCommand>(controller::ManualMoveCommand::Direction::kWest, _step_size, GetReflectorsToMove(_reflectors_data)));
  }

  ImGui::InputDouble("Step size (°)", &_step_size);

  ImGui::BeginGroup();

  std::size_t columns = _reflectors_data.size() / 10 + 1;
  std::size_t reflector_index = 0;
  for (std::size_t i = 0; i < columns; ++i) {
    ImGui::BeginGroup();
    for (std::size_t j = 0; j < 10 && reflector_index < _reflectors_data.size(); ++j) {
      std::string name = "REF-" + std::to_string(_reflectors_data[reflector_index].id);
      ImGui::Checkbox(name.c_str(), &(_reflectors_data[reflector_index].to_move));
      ++reflector_index;
    }
    ImGui::EndGroup();

    if (columns != _reflectors_data.size() / 10) { ImGui::SameLine(); }
  }

  ImGui::EndGroup();

  ImGui::EndChild();
  ImGui::EndGroup();
}

std::vector<std::size_t> ManualReflectorsAlignmentWindowComponent::GetReflectorsToMove(const std::vector<ReflectorData>& reflectors_data) {
  std::vector<std::size_t> result;
  for (std::size_t i = 0; i < reflectors_data.size(); ++i) {
    if (reflectors_data[i].to_move) { result.push_back(i); }
  }
  return result;
}

}  // namespace voltiris::presentation::ui
