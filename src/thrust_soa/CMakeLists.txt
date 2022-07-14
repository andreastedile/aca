add_library(cuda_soa
        thrust_soa.cu
        thrust_soa.cuh)

target_link_libraries(cuda_soa INTERFACE quadtree)

thrust_create_target(Thrust_cuda_soa)
target_link_libraries(cuda_soa PUBLIC Thrust_cuda_soa) # why not PRIVATE?
