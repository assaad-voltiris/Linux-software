#pragma once

#include <string>

#include <imgui.h>

namespace voltiris {

class Resources{
public:
  // UI
  // Values
  static const std::size_t kWindowWidth;
  static const std::size_t kWindowHeight;

  static const ImVec4 kBackgroundColor;

  // String
  static const char* kWindowTitle;

  // DEFAULT VALUES
  static const std::int32_t kATSCycleFrequencyDefaultIndex;
  static const double kATSStartingHraDefaultValue;
  static const std::int32_t kATSAccelerationFactorDefaultIndex;
  static const bool k400m0DefaultChecked;
  static const bool kToThPosDefaultChecked;

  static const double kMRAStepSizeDefaultValue;

  static const double kASTLatitudeDefaultValue;
  static const double kASTLongitudeDefaultValue;
  static const std::int32_t kASTCountCyclesDefaultValue;


};
}