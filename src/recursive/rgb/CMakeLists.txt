add_library(rgb rgb.h rgbsoa.cpp rgbsoa.h)

target_link_libraries(rgb PUBLIC Eigen3::Eigen)

target_include_directories(rgb INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
