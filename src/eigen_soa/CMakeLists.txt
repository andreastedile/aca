add_library(eigen_soa
        eigen_soa.cpp
        eigen_soa.h)

target_link_libraries(eigen_soa INTERFACE quadtree)

target_link_libraries(eigen_soa PRIVATE Eigen3::Eigen)
