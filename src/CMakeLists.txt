add_library(libquadtree
    ${PROJECT_SOURCE_DIR}/include/hello.hpp
    hello.cpp
    ${PROJECT_SOURCE_DIR}/include/image.hpp
    image.cpp
    ${PROJECT_SOURCE_DIR}/include/quadtree.hpp
    quadtree.cpp)

target_include_directories(libquadtree PUBLIC "${stb_SOURCE_DIR}")

target_include_directories(libquadtree PUBLIC ${PROJECT_SOURCE_DIR}/include)
