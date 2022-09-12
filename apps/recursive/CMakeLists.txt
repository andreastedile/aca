add_executable(qt-rec-bu qt_rec_bu.cpp)

target_link_libraries(qt-rec-bu PRIVATE rgb)
target_link_libraries(qt-rec-bu PRIVATE quadtree)
target_link_libraries(qt-rec-bu PRIVATE quadrant)
target_link_libraries(qt-rec-bu PRIVATE construction)
target_link_libraries(qt-rec-bu PRIVATE colorization)
target_link_libraries(qt-rec-bu PRIVATE util)
target_link_libraries(qt-rec-bu PRIVATE io)

target_link_libraries(qt-rec-bu PRIVATE argparse::argparse)
target_link_libraries(qt-rec-bu PRIVATE spdlog::spdlog)
target_link_libraries(qt-rec-bu PRIVATE stb::stb)


add_executable(qt-rec-td qt_rec_td.cpp)

target_link_libraries(qt-rec-td PRIVATE rgb)
target_link_libraries(qt-rec-td PRIVATE quadtree)
target_link_libraries(qt-rec-td PRIVATE quadrant)
target_link_libraries(qt-rec-td PRIVATE construction)
target_link_libraries(qt-rec-td PRIVATE colorization)
target_link_libraries(qt-rec-td PRIVATE util)
target_link_libraries(qt-rec-td PRIVATE io)

target_link_libraries(qt-rec-td PRIVATE argparse::argparse)
target_link_libraries(qt-rec-td PRIVATE spdlog::spdlog)
target_link_libraries(qt-rec-td PRIVATE stb::stb)
