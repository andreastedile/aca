add_library(cuda_quadrant
        cuda_quadrant.cu
        cuda_quadrant.cuh)

target_link_libraries(cuda_quadrant INTERFACE quadtree)

set_property(TARGET cuda_quadrant PROPERTY CUDA_STANDARD 17)

thrust_create_target(Thrust_cuda_quadrant)
target_link_libraries(cuda_quadrant PRIVATE Thrust_cuda_quadrant)
