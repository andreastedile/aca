cmake_minimum_required(VERSION 3.14)
project(aca CXX CUDA)
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS  OFF)

set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(stb
    GIT_REPOSITORY https://github.com/nothings/stb
)
FetchContent_MakeAvailable(stb)

find_package(Eigen3 3.4 REQUIRED NO_MODULE)

find_package(Thrust REQUIRED CONFIG)

add_subdirectory(src)
# add_subdirectory(src-cuda)
add_subdirectory(apps)
