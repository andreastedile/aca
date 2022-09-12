add_library(util padding.cpp padding.h qtmath.h)

set_target_properties(util PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

target_include_directories(util INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
