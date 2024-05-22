#pragma once

#include <memory>

#include <GLFW/glfw3.h>

namespace voltiris::presentation::ui {

class STWindow;

class Renderer {
public:
  explicit Renderer() = default;
  ~Renderer() = default;

  bool Init();
  bool Cleanup();

  bool Run(STWindow& st_window);

private:
  void FramePreprocessing();
  void FramePostprocessing();

private:
  float _current_scale = 1.;
  GLFWwindow* _window = nullptr;
};

}  // namespace voltiris::presentation::ui