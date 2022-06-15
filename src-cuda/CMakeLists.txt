add_executable(app-cuda app-cuda.cu)

target_include_directories(app-cuda PUBLIC "${stb_SOURCE_DIR}")

# https://cmake.org/cmake/help/latest/prop_tgt/CUDA_ARCHITECTURES.html
set_target_properties(app-cuda PROPERTIES CUDA_ARCHITECTURES native)
