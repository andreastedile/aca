add_library(util padding.cpp padding.h)

target_include_directories(util INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
