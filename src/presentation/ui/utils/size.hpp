//
// Created by sergi on 5/22/2024.
//

#pragma once

#include <imgui.h>

namespace voltiris::presentation::ui::utils {

inline ImVec2 GetRealSize(const ImVec2 &original_size, float scale) { return {original_size.x * scale, original_size.y * scale}; }

}  // namespace voltiris::presentation::ui
