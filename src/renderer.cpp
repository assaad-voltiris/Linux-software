#include "renderer.hpp"

#include <spdlog/spdlog.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>

#include <resources.hpp>
#include <st_window.hpp>

namespace voltiris {

namespace {

static void GlfwErrorCallback(int error, const char *description) {
  spdlog::error("Glfw Error {}: {}", error, description);
}

} // namespace

bool Renderer::Init() {
  glfwSetErrorCallback(GlfwErrorCallback);

  if (!glfwInit()) {
    return false;
  }

  _window = glfwCreateWindow(
      static_cast<std::int32_t>(voltiris::Resources::kWindowWidth),
      static_cast<std::int32_t>(voltiris::Resources::kWindowHeight),
      voltiris::Resources::kWindowTitle, nullptr, nullptr);
  if (_window == nullptr) {
    return false;
  }

  glfwMakeContextCurrent(_window);
  glfwSwapInterval(1); // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsClassic();
  //ImGui::StyleColorsLight();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(_window, true);
  ImGui_ImplOpenGL2_Init();

  return true;
}

bool Renderer::Cleanup() {
  ImGui_ImplOpenGL2_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(_window);
  glfwTerminate();

  return true;
}

bool Renderer::Run(STWindow& st_window) {
  // Main rendering loop
  while (!glfwWindowShouldClose(_window)) {
    FramePreprocessing();

    st_window.Render(_current_scale);

    FramePostprocessing();
  }

  return true;
}

void Renderer::FramePreprocessing() {
  glfwPollEvents();

  auto &io = ImGui::GetIO();

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL2_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  float x_scale, y_scale;
  glfwGetWindowContentScale(_window, &x_scale, &y_scale);

  std::int32_t x_size, y_size;
  glfwGetFramebufferSize(_window, &x_size, &y_size);

  if(_current_scale != std::max(x_scale, y_scale)) {
    io.FontGlobalScale = std::max(x_scale, y_scale);

    auto& style = ImGui::GetStyle();
    ImGui::GetStyle().ScaleAllSizes(std::max(x_scale, y_scale) / _current_scale);

    _current_scale = std::max(x_scale, y_scale);
  }

  //io.DisplaySize = {static_cast<float>(x_size), static_cast<float>(y_size)};

  //io.FontGlobalScale = std::max(x_scale, y_scale);
  //io.DisplayFramebufferScale = {x_scale, y_scale};

#ifdef IMGUI_HAS_VIEWPORT
  ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->GetWorkPos());
  ImGui::SetNextWindowSize(viewport->GetWorkSize());
  ImGui::SetNextWindowViewport(viewport->ID);
#else
  ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
  ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
#endif
}

void Renderer::FramePostprocessing() {
  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(_window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(voltiris::Resources::kBackgroundColor.x,
               voltiris::Resources::kBackgroundColor.y,
               voltiris::Resources::kBackgroundColor.z,
               voltiris::Resources::kBackgroundColor.w);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

  glfwMakeContextCurrent(_window);
  glfwSwapBuffers(_window);
}

} // namespace voltiris
