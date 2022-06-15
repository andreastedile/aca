add_executable(app-cuda app-cuda.cu)

target_include_directories(app-cuda PUBLIC "${stb_SOURCE_DIR}")
