add_library(construction bottom_up.cpp bottom_up.h top_down.cpp top_down.h)

target_link_libraries(construction PUBLIC rgb)
target_link_libraries(construction PUBLIC quadtree)
target_link_libraries(construction PUBLIC quadrant)
target_link_libraries(construction PUBLIC util)

target_link_libraries(construction PRIVATE spdlog::spdlog)

if (LOG_CONSTRUCTION)
    target_compile_definitions(construction PUBLIC LOG_CONSTRUCTION)
endif ()

if (NPARALLEL)
    target_compile_definitions(construction PUBLIC NPARALLEL)
endif ()

target_include_directories(construction INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
