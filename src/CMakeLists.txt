# Abstract quadtree
add_subdirectory(quadtree)
add_subdirectory(colorizer) # uses quadtree

# Eigen quadtree
add_subdirectory(eigen_soa) # uses quadtree
add_subdirectory(eigen_quadrant) # uses quadtree, eigen_soa

# Cuda quadtree
add_subdirectory(thrust_soa) # uses quadtree
add_subdirectory(thrust_quadrant) # uses quadtree, thrust_soa

target_include_directories(quadtree INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_include_directories(eigen_soa INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_include_directories(eigen_quadrant INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_include_directories(colorizer INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)