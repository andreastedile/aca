add_library(quadtree
        rgb.h
        soa.h
        quadtree.cpp
        quadtree.h
        quadrant.h
        top_down.h)

if (LOG_QUADTREE_BUILD)
    target_compile_definitions(quadtree PRIVATE LOG_QUADTREE_BUILD)
endif ()

if (PARALLEL)
    # Fixme: why PUBLIC and not PRIVATE?
    target_compile_definitions(quadtree PUBLIC PARALLEL)
    find_package(Threads REQUIRED)
    target_link_libraries(quadtree PRIVATE Threads::Threads)
endif ()
