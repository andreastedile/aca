#ifndef ACA_CUDA_SOA_H
#define ACA_CUDA_SOA_H

#include "../quadtree/rgb.h" // color_t
#include "../quadtree/soa.h" // PixelSoa

#include <cstdint> // uint8_t
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>

using CudaHostPixelArray = thrust::host_vector<uint8_t>;
using CudaDevicePixelArray = thrust::device_vector<uint8_t>;

using CudaHostPixelSoa = PixelSoa<CudaDevicePixelArray>;
using CudaDevicePixelSoa = PixelSoa<CudaDevicePixelArray>;

CudaDevicePixelSoa to_cuda_pixel_soa(const uint8_t* pixels, int n_pixels);

#endif // ACA_CUDA_SOA_H
