add_library(io io.h io.cpp)

target_compile_definitions(io PUBLIC STB_IMAGE_IMPLEMENTATION)
target_compile_definitions(io PUBLIC STB_IMAGE_WRITE_IMPLEMENTATION)
target_link_libraries(io PRIVATE util)
target_link_libraries(io PRIVATE spdlog::spdlog)
target_link_libraries(io PRIVATE stb::stb)

target_include_directories(
  io INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
