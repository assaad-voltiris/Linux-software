import os
from conan import ConanFile
from conan.tools.files import copy
from conan.tools.cmake import cmake_layout, CMake


class VoltirisST(ConanFile):
    name = "voltiris-st"
    description = "Sun tracking control system by Voltiris"
    license = "None"
    url = "None"
    generators = "CMakeDeps", "CMakeToolchain"

    settings = "os", "arch", "compiler", "build_type"

    requires = [
        "imgui/1.90.5",
        "glfw/3.4",
        "spdlog/1.14.0",
        "websocketpp/0.8.2",
        "nlohmann_json/3.11.3"
    ]

    def layout(self):
        cmake_layout(self)

    def generate(self):
        imgui_srcdir = self.dependencies["imgui"].cpp_info.srcdirs[0]
        imgui_impl_target_dir = os.path.join(self.source_folder, "bindings")
        copy(self, "imgui_impl_glfw.cpp", imgui_srcdir, imgui_impl_target_dir)
        copy(self, "imgui_impl_opengl2.cpp", imgui_srcdir, imgui_impl_target_dir)
        copy(self, "imgui_impl_glfw.h", imgui_srcdir, imgui_impl_target_dir)
        copy(self, "imgui_impl_opengl2.h", imgui_srcdir, imgui_impl_target_dir)

    def build(self):
        cmake = CMake(self)

        cmake.configure()
        cmake.build()
