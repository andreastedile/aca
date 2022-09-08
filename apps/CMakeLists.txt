add_executable(app app.cpp)

target_link_libraries(app PRIVATE rgb)
target_link_libraries(app PRIVATE quadtree)
target_link_libraries(app PRIVATE quadrant)
target_link_libraries(app PRIVATE construction)
target_link_libraries(app PRIVATE colorizer)

target_compile_definitions(app PUBLIC STB_IMAGE_IMPLEMENTATION)
target_compile_definitions(app PUBLIC STB_IMAGE_WRITE_IMPLEMENTATION)

target_link_libraries(app PRIVATE argparse::argparse)
target_link_libraries(app PRIVATE spdlog::spdlog)
target_link_libraries(app PRIVATE stb::stb)