FROM balenalib/beaglebone-black-debian:latest

RUN apt update && apt upgrade

RUN apt install -y cmake pkg-config build-essential python3 python3-pip git gcc-10

RUN pip install conan

RUN conan profile detect

RUN conan install --requires imgui/1.90.5 --requires glfw/3.4 --requires spdlog/1.14.0 --requires websocketpp/0.8.2 --requires nlohmann_json/3.11.3 --build=missing \
    -c tools.system.package_manager:mode=install