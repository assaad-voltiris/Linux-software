# Install build tools

1. We need `cmake` as build system and `conan` as dependencies manager. Also `pkg-config` is required by some of the dependencies
   ```
   sudo apt install cmake pkg-config
   pip install conan --break-system-packages
   ```
   
# Install dependencies

1. Only one dependency for file choosing dialog is kdialog
   ```sudo apt install kdialog```

# Build project

1. Clone project ```git clone git@github.com:assaad-voltiris/Linux-software.git```
2. Navigate to project folder
3. Build project ```conan build . -of build --build=missing -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True```
4. Run project ```./build/build/Release/VoltirisST```