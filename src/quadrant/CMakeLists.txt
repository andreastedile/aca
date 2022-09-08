add_library(quadrant quadrant.cpp quadrant.h)

target_link_libraries(quadrant PUBLIC rgb)

target_include_directories(quadrant INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
