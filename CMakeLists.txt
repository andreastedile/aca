cmake_minimum_required(VERSION 3.14)
project(aca CXX CUDA)
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

find_package(Eigen3 3.4 REQUIRED)
find_package(stb REQUIRED)
#find_package(Thrust REQUIRED)

add_subdirectory(src)
add_subdirectory(apps)
