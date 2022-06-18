add_library(eigen_quadtree
        eigen_quadtree.cpp
        eigen_quadtree.h)

target_include_directories(eigen_quadtree PRIVATE "${stb_SOURCE_DIR}")

target_link_libraries(eigen_quadtree INTERFACE eigen_soa)
