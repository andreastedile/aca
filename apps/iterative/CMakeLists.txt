add_executable(qt-it-gpu qt_it_gpu.cu cuda_check.h)

set_target_properties(qt-it-gpu PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

target_link_libraries(qt-it-gpu PRIVATE irgb)
target_link_libraries(qt-it-gpu PRIVATE inode)
target_link_libraries(qt-it-gpu PRIVATE iconstruction)
target_link_libraries(qt-it-gpu PRIVATE icolorization)
target_link_libraries(qt-it-gpu PRIVATE util)

target_compile_definitions(qt-it-gpu PUBLIC STB_IMAGE_IMPLEMENTATION)
target_compile_definitions(qt-it-gpu PUBLIC STB_IMAGE_WRITE_IMPLEMENTATION)

target_link_libraries(qt-it-gpu PRIVATE argparse::argparse)
target_link_libraries(qt-it-gpu PRIVATE spdlog::spdlog)
target_link_libraries(qt-it-gpu PRIVATE stb::stb)
target_link_libraries(qt-it-gpu PRIVATE OpenMP::OpenMP_CXX)
