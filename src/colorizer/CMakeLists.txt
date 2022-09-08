add_library(colorizer colorizer.cpp colorizer.h)

target_link_libraries(colorizer PRIVATE rgb)
target_link_libraries(colorizer PUBLIC quadtree)

target_include_directories(colorizer INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
