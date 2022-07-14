add_library(thrust_quadrant
        thrust_quadrant.cu
        thrust_quadrant.cuh)

target_link_libraries(thrust_quadrant INTERFACE quadtree)
target_link_libraries(thrust_quadrant INTERFACE thrust_soa)

set_property(TARGET thrust_quadrant PROPERTY CUDA_STANDARD 17)

thrust_create_target(Thrust_cuda_quadrant)
target_link_libraries(thrust_quadrant PRIVATE Thrust_cuda_quadrant)
