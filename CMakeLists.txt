cmake_minimum_required(VERSION 3.14)
project(aca CXX)

include(FetchContent)
FetchContent_Declare(stb
    GIT_REPOSITORY https://github.com/nothings/stb
)
FetchContent_MakeAvailable(stb)

add_subdirectory(src)
add_subdirectory(apps)
