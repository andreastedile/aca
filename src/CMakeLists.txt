# Abstract quadtree
add_subdirectory(quadtree)

# Eigen quadtree
add_subdirectory(eigen_soa) # uses quadtree
add_subdirectory(eigen_quadtree) # uses quadtree, eigen_soa
add_subdirectory(colorizer) # uses quadtree

target_include_directories(quadtree INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_include_directories(eigen_soa INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_include_directories(eigen_quadtree INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_include_directories(colorizer INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)