add_library(irgb rgb.h rgbaos.cpp rgbaos.h)

set_target_properties(irgb PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

target_include_directories(irgb INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
