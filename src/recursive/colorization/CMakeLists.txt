add_library(colorization colorization.cpp colorization.h)

target_link_libraries(colorization PUBLIC quadtree)

target_include_directories(colorization INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
