add_library(quadtree quadtree.cpp quadtree.h)

target_link_libraries(quadtree PUBLIC rgb)

target_link_libraries(quadtree PRIVATE spdlog::spdlog)
target_link_libraries(quadtree PRIVATE Threads::Threads)

if (LOG_QUADTREE)
    target_compile_definitions(quadtree PUBLIC LOG_QUADTREE)
endif ()

target_include_directories(quadtree INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
