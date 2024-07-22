cmake_minimum_required(VERSION 3.18)

project(VoltirisST VERSION 1.0.0 LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        src/main.cpp

        src/client_sun_tracking_app.cpp

        src/utils/frame_limiter.cpp

        src/presentation/client/client.cpp

        src/controller/commands/reflectors_controller_command.cpp
        src/controller/commands/connect_command.cpp
        src/controller/commands/disconnect_command.cpp
        src/controller/commands/flash_command.cpp
        src/controller/commands/go_command.cpp
        src/controller/commands/initialization_command.cpp
        src/controller/commands/load_configuration_command.cpp
        src/controller/commands/manual_move_command.cpp
        src/controller/commands/read_command.cpp
        src/controller/commands/reboot_command.cpp
        src/controller/commands/request_configuration_command.cpp
        src/controller/commands/set_position_command.cpp
        src/controller/commands/values_update_command.cpp
        src/controller/commands/start_tracking_command.cpp
        src/controller/commands/stop_tracking_command.cpp

        src/controller/updates/reflectors_controller_update.cpp
        src/controller/updates/ast_update.cpp
        src/controller/updates/bus_connected_update.cpp
        src/controller/updates/bus_disconnected_update.cpp
        src/controller/updates/bus_list_update.cpp
        src/controller/updates/cp_update.cpp
        src/controller/updates/current_configuration_update.cpp
        src/controller/updates/reflector_state_update.cpp
        src/controller/updates/reflectors_size_update.cpp
        src/controller/updates/error_update.cpp
        src/controller/updates/status_update.cpp

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
        src/presentation/ui/window_components/controller_status_window_component.cpp

        # ImGui impl bindings
        bindings/imgui_impl_glfw.cpp
        bindings/imgui_impl_opengl2.cpp
)

if (NOT WIN32)
    target_sources(${PROJECT_NAME} PRIVATE
            src/standalone_sun_tracking_app.cpp
            src/server_sun_tracking_app.cpp

            src/presentation/server/server.cpp

            src/controller/reflectors_controller.cpp
            src/controller/reflectors_controller_commands_processor.cpp
            src/controller/utils/communication.cpp
            src/controller/utils/converters.cpp
            src/controller/utils/reflector.cpp
            src/controller/utils/sun_math.cpp
            src/controller/utils/time.cpp
    )
endif ()

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

target_include_directories(${PROJECT_NAME} PRIVATE bindings src)
target_include_directories(${PROJECT_NAME} PRIVATE 3rdparty/pfd)

find_package(glfw3 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

find_package(imgui REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

find_package(spdlog REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)

find_package(websocketpp REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE websocketpp::websocketpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::random)

find_package(nlohmann_json REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)