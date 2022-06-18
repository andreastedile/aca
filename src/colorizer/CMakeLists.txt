add_library(colorizer
        colorizer.cpp
        colorizer.h)

target_link_libraries(colorizer INTERFACE quadtree)
