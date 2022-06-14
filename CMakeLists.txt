cmake_minimum_required(VERSION 3.14)
project(aca CXX CUDA)
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

include(FetchContent)
FetchContent_Declare(stb
    GIT_REPOSITORY https://github.com/nothings/stb
)
FetchContent_MakeAvailable(stb)

add_subdirectory(src)
add_subdirectory(src-cuda)
add_subdirectory(apps)
