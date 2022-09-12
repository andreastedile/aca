add_executable(qt-rec qt_rec.cpp)

target_link_libraries(qt-rec PRIVATE rgb)
target_link_libraries(qt-rec PRIVATE quadtree)
target_link_libraries(qt-rec PRIVATE quadrant)
target_link_libraries(qt-rec PRIVATE construction)
target_link_libraries(qt-rec PRIVATE colorization)
target_link_libraries(qt-rec PRIVATE util)
target_link_libraries(qt-rec PRIVATE io)

target_link_libraries(qt-rec PRIVATE argparse::argparse)
target_link_libraries(qt-rec PRIVATE spdlog::spdlog)
target_link_libraries(qt-rec PRIVATE stb::stb)
