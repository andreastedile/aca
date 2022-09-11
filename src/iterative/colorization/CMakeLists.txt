add_library(icolorization colorization.cpp colorization.h)

set_target_properties(icolorization PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

target_link_libraries(icolorization PUBLIC inode)

target_include_directories(
  icolorization INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
