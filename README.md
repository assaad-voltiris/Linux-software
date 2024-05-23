# Install build tools

1. We need `cmake` as build system and `conan` as dependencies manager. Also `pkg-config` is required by some of the dependencies
   ```
   sudo apt install cmake pkg-config
   pip install conan --break-system-packages
   ```[build.sh](scripts%2Fbuild.sh)
   
# Install dependencies

1. Only one dependency for file choosing dialog is kdialog
   ```sudo apt install kdialog```

# Build project
1. (Optional) Create default conan profile (```conan profile detect```)
2. Clone project ```git clone https://github.com/assaad-voltiris/Linux-software.git```
3. Navigate to project folder
4. Build project ```conan build . -of build --build=missing -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True```
5. Run project ```./build/build/Release/VoltirisST```