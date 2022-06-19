add_library(quadtree
        rgb.h
        soa.h
        quadtree.cpp
        quadtree.h)

if (LOG_QUADTREE_BUILD)
    target_compile_definitions(quadtree PRIVATE LOG_QUADTREE_BUILD)
endif ()
