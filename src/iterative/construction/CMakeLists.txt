add_library(iconstruction
        combination.cu combination.h
        construction_host.cpp construction_host.h
        construction_device.cu construction_device.cuh)

# Building CUDA object src/iterative/construction/CMakeFiles/iconstruction.dir/construction_device.cu.o
# FAILED
# ptxas fatal   : Unresolved extern function '_Z18make_internal_nodeR4NodeS0_S0_S0_f'
set_target_properties(iconstruction PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

target_link_libraries(iconstruction PUBLIC rgb)
target_link_libraries(iconstruction PUBLIC inode)
target_link_libraries(iconstruction PRIVATE util)

target_link_libraries(iconstruction PRIVATE OpenMP::OpenMP_CXX)
target_link_libraries(iconstruction PRIVATE spdlog::spdlog)

if (DEBUG_CONSTRUCT_QUADTREE_HOST)
    target_compile_definitions(iconstruction PUBLIC DEBUG_CONSTRUCT_QUADTREE_HOST)
endif ()

if (NPARALLEL)
    target_compile_definitions(iconstruction PUBLIC NPARALLEL)
endif ()

target_include_directories(iconstruction INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
