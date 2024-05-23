#include "renderer.hpp"

#include <spdlog/spdlog.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>

#include <presentation/ui/resources.hpp>
#include <presentation/ui/st_window.hpp>

namespace voltiris::presentation::ui {

namespace {

void GlfwErrorCallback(int error, const char *description) { spdlog::error("Glfw Error {}: {}", error, description); }

}  // namespace

bool Renderer::Init() {
  glfwSetErrorCallback(GlfwErrorCallback);

  if (!glfwInit()) { return false; }

  _window = glfwCreateWindow(static_cast<std::int32_t>(Resources::kWindowWidth), static_cast<std::int32_t>(Resources::kWindowHeight), Resources::kWindowTitle,
                             nullptr, nullptr);
  if (_window == nullptr) { return false; }

  glfwMakeContextCurrent(_window);
  glfwSwapInterval(1);  // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();
  // ImGui::StyleColorsLight();

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

bool Renderer::Run(STWindow &st_window) {
  const auto max_frame_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)) / 10;

  // Main rendering loop
  while (!glfwWindowShouldClose(_window)) {
    auto start = std::chrono::high_resolution_clock::now();

    FramePreprocessing();

    st_window.Render(_current_scale);

    FramePostprocessing();

    auto frame_time = std::chrono::high_resolution_clock::now() - start;
    auto sleep_time = max_frame_time - frame_time;

    if (sleep_time.count() > 0) {
      // spdlog::info("Sleep renderer for {} ns.", sleep_time.count());
      std::this_thread::sleep_for(sleep_time);
    }
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

  if (_current_scale != std::max(x_scale, y_scale)) {
    io.FontGlobalScale = std::max(x_scale, y_scale);

    auto &style = ImGui::GetStyle();
    ImGui::GetStyle().ScaleAllSizes(std::max(x_scale, y_scale) / _current_scale);

    _current_scale = std::max(x_scale, y_scale);
  }

  ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
  ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
}

void Renderer::FramePostprocessing() {
  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(_window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(Resources::kBackgroundColor.x, Resources::kBackgroundColor.y, Resources::kBackgroundColor.z, Resources::kBackgroundColor.w);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

  glfwMakeContextCurrent(_window);
  glfwSwapBuffers(_window);
}

}  // namespace voltiris::presentation::ui
