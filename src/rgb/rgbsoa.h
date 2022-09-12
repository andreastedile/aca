#pragma once

#include <Eigen/Eigen>

using EigenArray = Eigen::Array<unsigned char, 1, Eigen::Dynamic>;

struct RGBSoA {
    int n_elements;
    EigenArray r;
    EigenArray g;
    EigenArray b;
};

RGBSoA flatten_to_rgb_soa(const unsigned char* pixels, int n_rows, int n_cols);
