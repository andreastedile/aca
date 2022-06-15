add_library(libquadtree
        image.hpp
        image.cpp
        quadtree.hpp
        quadtree.cpp
        rgb.hpp
        rgb.cpp
        quadtree_with_eigen.cpp
        quadtree_with_eigen.h
        flatten.h
        file_utils.h)

if (PARALLEL)
    target_compile_definitions(libquadtree PRIVATE PARALLEL)
endif ()

target_include_directories(libquadtree PUBLIC "${stb_SOURCE_DIR}")
target_link_libraries(libquadtree PRIVATE Eigen3::Eigen)

target_include_directories(libquadtree INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

find_package(Threads REQUIRED)
target_link_libraries(libquadtree PRIVATE Threads::Threads)
