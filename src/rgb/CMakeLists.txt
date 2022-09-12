add_library(rgb rgb.h rgbaos.cpp rgbaos.h rgbsoa.cpp rgbsoa.h)

set_target_properties(rgb PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

target_link_libraries(rgb PUBLIC Eigen3::Eigen)

target_include_directories(rgb INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
