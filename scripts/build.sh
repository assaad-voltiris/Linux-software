git clone https://github.com/assaad-voltiris/Linux-software.git

cd Linux-software

conan build . -of build -s compiler.cppstd=17 --build=missing -c tools.system.package_manager:mode=install