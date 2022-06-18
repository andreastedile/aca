#ifndef ACA_EIGEN_SOA_H
#define ACA_EIGEN_SOA_H

#include "../quadtree/rgb.h" // color_t
#include "../quadtree/soa.h" // PixelSoa

#include <cstdint> // uint8_t
#include <eigen3/Eigen/Eigen>

using EigenPixelArray = Eigen::Array<color_t, 1, Eigen::Dynamic>;
using EigenPixelSoa = PixelSoa<EigenPixelArray>;

EigenPixelSoa to_eigen_pixel_soa(const uint8_t* pixels, int n_pixels);

#endif // ACA_EIGEN_SOA_H
