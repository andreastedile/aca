#ifndef ACA_RGBSOA_H
#define ACA_RGBSOA_H

#include <Eigen/Eigen>

using EigenArray = Eigen::Array<unsigned char, 1, Eigen::Dynamic>;

struct RGBSoA {
    EigenArray r;
    EigenArray g;
    EigenArray b;
};

RGBSoA flatten_to_rgb_soa(const unsigned char* pixels, int n_pixels);

#endif // ACA_RGBSOA_H
