add_library(eigen_quadtree
        eigen_quadtree.cpp
        eigen_quadtree.h)

target_link_libraries(eigen_quadtree INTERFACE eigen_soa)
