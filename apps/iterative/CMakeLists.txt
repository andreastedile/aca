add_executable(qt-it-gpu qt_it_gpu.cu cuda_check.h)

set_target_properties(qt-it-gpu PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

target_link_libraries(qt-it-gpu PRIVATE irgb)
target_link_libraries(qt-it-gpu PRIVATE inode)
target_link_libraries(qt-it-gpu PRIVATE iconstruction)
target_link_libraries(qt-it-gpu PRIVATE icolorization)
target_link_libraries(qt-it-gpu PRIVATE util)
target_link_libraries(qt-it-gpu PRIVATE io)

target_link_libraries(qt-it-gpu PRIVATE argparse::argparse)
target_link_libraries(qt-it-gpu PRIVATE spdlog::spdlog)
target_link_libraries(qt-it-gpu PRIVATE OpenMP::OpenMP_CXX)


add_executable(qt-it-cpu qt_it_cpu.cu)

target_link_libraries(qt-it-cpu PRIVATE irgb)
target_link_libraries(qt-it-cpu PRIVATE inode)
target_link_libraries(qt-it-cpu PRIVATE iconstruction)
target_link_libraries(qt-it-cpu PRIVATE icolorization)
target_link_libraries(qt-it-cpu PRIVATE util)
target_link_libraries(qt-it-cpu PRIVATE io)

target_link_libraries(qt-it-cpu PRIVATE argparse::argparse)
target_link_libraries(qt-it-cpu PRIVATE spdlog::spdlog)
target_link_libraries(qt-it-cpu PRIVATE OpenMP::OpenMP_CXX)
