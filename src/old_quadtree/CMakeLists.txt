add_library(old_quadtree
        rgb.cpp
        rgb.cpp
        image.cpp
        image.hpp
        old_quadtree.cpp
        old_quadtree.hpp)

if (PARALLEL)
    target_compile_definitions(old_quadtree PRIVATE PARALLEL)
endif ()

target_include_directories(old_quadtree PRIVATE "${stb_SOURCE_DIR}")

find_package(Threads REQUIRED)
target_link_libraries(old_quadtree PRIVATE Threads::Threads)
