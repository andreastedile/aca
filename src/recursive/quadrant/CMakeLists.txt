add_library(quadrant
        quadrant.cpp quadrant.h
        soa_ops.cpp soa_ops.h
        aos_ops.cpp aos_ops.h)

target_link_libraries(quadrant PUBLIC rgb)

target_include_directories(quadrant INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
