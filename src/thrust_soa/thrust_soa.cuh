#ifndef ACA_CUDA_SOA_H
#define ACA_CUDA_SOA_H

#include "../quadtree/rgb.h" // color_t
#include "../quadtree/soa.h" // PixelSoa

#include <cstdint> // uint8_t
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>

using ThrustHostPixelArray = thrust::host_vector<uint8_t>;
using ThrustDevicePixelArray = thrust::device_vector<uint8_t>;

using ThrustHostPixelSoa = PixelSoa<ThrustDevicePixelArray>;
using ThrustDevicePixelSoa = PixelSoa<ThrustDevicePixelArray>;

ThrustDevicePixelSoa to_thrust_pixel_soa(const uint8_t* pixels, int n_pixels);

#endif // ACA_CUDA_SOA_H
