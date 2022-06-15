cmake_minimum_required(VERSION 3.24)
project(aca CXX CUDA)
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

include(FetchContent)
FetchContent_Declare(stb
    GIT_REPOSITORY https://github.com/nothings/stb
)
FetchContent_MakeAvailable(stb)

find_package(Eigen3 3.4 REQUIRED NO_MODULE)

add_subdirectory(src)
add_subdirectory(src-cuda)
add_subdirectory(apps)
