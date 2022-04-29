add_library(libquadtree
    ${PROJECT_SOURCE_DIR}/include/hello.h
    hello.cc)

target_include_directories(libquadtree PUBLIC ${PROJECT_SOURCE_DIR}/include)
