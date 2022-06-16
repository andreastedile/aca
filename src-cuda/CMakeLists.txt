add_executable(app-cuda app-cuda.cu)

target_include_directories(app-cuda PUBLIC "${stb_SOURCE_DIR}")

# https://cmake.org/cmake/help/latest/prop_tgt/CUDA_ARCHITECTURES.html
# Fixme: find equivalent for older CMake versions
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.24")
    message("setting CUDA_ARCHITECTURES")
    set_target_properties(app-cuda PROPERTIES CUDA_ARCHITECTURES native)
endif()
