mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=~/Documents/vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake ..
cmake --build . --config Release
copy /Y Release\main.exe ..
pause