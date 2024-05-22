#pragma once

#include <cstdint>
#include <string>

#include <imgui.h>

namespace voltiris::presentation::ui {

class Resources {
public:
  // UI
  // Values
  static const std::size_t kWindowWidth;
  static const std::size_t kWindowHeight;

  static const ImVec4 kBackgroundColor;

  // String
  static const char* kWindowTitle;

  // DEFAULT VALUES
  static const double kATSStartingHraDefaultValue;

  static const double kMRAStepSizeDefaultValue;

  static const double kASTLatitudeDefaultValue;
  static const double kASTLongitudeDefaultValue;
  static const std::int32_t kASTCountCyclesDefaultValue;
};
}  // namespace voltiris