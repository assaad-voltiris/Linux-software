cmake_minimum_required(VERSION 3.18)

project(VoltirisST VERSION 1.0.0 LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        src/main.cpp

        src/renderer.cpp
        src/st_window.cpp
        src/sun_tracking_app.cpp
        src/reflectors_controller.cpp
        src/rs485.cpp
        src/arduino_mega.cpp

        src/resources.cpp
        # ImGui impl bindings
        bindings/imgui_impl_glfw.cpp
        bindings/imgui_impl_opengl2.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE bindings src)
target_include_directories(${PROJECT_NAME} PRIVATE 3rdparty/pfd)

find_package(glfw3 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

find_package(imgui REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

find_package(spdlog REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)