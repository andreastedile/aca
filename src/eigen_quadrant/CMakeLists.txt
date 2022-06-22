add_library(eigen_quadrant
        eigen_quadrant.cpp
        eigen_quadrant.h)

target_link_libraries(eigen_quadrant INTERFACE quadtree)
target_link_libraries(eigen_quadrant INTERFACE eigen_soa)
