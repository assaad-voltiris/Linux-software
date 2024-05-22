#include "resources.hpp"

namespace voltiris::presentation::ui {

// UI
// Values
const std::size_t Resources::kWindowWidth = 1280;
const std::size_t Resources::kWindowHeight = 720;

const ImVec4 Resources::kBackgroundColor = ImVec4(0.f, 0.f, 0.f, 1.00f);

// Strings
const char *Resources::kWindowTitle = "Voltiris - Sun Tracking Control";

// DEFAULT VALUES
const double Resources::kATSStartingHraDefaultValue = 200;

const double Resources::kMRAStepSizeDefaultValue = 5.0;

const double Resources::kASTLatitudeDefaultValue = 46.52;
const double Resources::kASTLongitudeDefaultValue = 6.63;
const std::int32_t Resources::kASTCountCyclesDefaultValue = 0;

}  // namespace voltiris::presentation::ui