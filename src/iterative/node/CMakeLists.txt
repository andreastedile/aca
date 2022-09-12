add_library(inode node.cu node.h)

set_target_properties(inode PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

target_link_libraries(inode PUBLIC rgb)

target_include_directories(inode INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
