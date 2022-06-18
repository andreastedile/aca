add_subdirectory(old_quadtree)
add_subdirectory(flattener)
add_subdirectory(colorizer)

add_library(libquadtree
        quadtree_with_eigen.cpp
        quadtree_with_eigen.h
        file_utils.h)

target_link_libraries(libquadtree PRIVATE colorizer)
target_link_libraries(libquadtree PRIVATE flattener)

target_include_directories(libquadtree PUBLIC "${stb_SOURCE_DIR}")
target_link_libraries(libquadtree PRIVATE Eigen3::Eigen)

target_include_directories(old_quadtree INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_include_directories(flattener INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_include_directories(colorizer INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
