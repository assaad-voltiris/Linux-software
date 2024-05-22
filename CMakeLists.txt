cmake_minimum_required(VERSION 3.18)

project(VoltirisST VERSION 1.0.0 LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/sun_tracking_app.cpp

        src/presentation/ui/renderer.cpp
        src/presentation/ui/resources.cpp
        src/presentation/ui/st_window.cpp
        src/presentation/ui/window_components/accelerated_tracking_settings_window_component.cpp
        src/presentation/ui/window_components/automatic_sun_tracking_window_component.cpp
        src/presentation/ui/window_components/calibration_position_window_component.cpp
        src/presentation/ui/window_components/configuration_window_component.cpp
        src/presentation/ui/window_components/error_window_component.cpp
        src/presentation/ui/window_components/manual_reflectors_alignment_window_component.cpp
        src/presentation/ui/window_components/menu_window_component.cpp
        src/presentation/ui/window_components/reflectors_data_window_component.cpp

        src/controller/reflectors_controller.cpp
        src/controller/utils/communication.cpp
        src/controller/utils/converters.cpp
        src/controller/utils/reflector.cpp
        src/controller/utils/sun_math.cpp
        src/controller/utils/time.cpp

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