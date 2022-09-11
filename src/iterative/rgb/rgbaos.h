#pragma once

#include "rgb.h"

struct RGBAoS {
    int n_elements;
    RGB<unsigned char>* aos;
    RGBAoS(RGB<unsigned char>* aos, int n_elements) : aos(aos), n_elements(n_elements) {}
};

RGBAoS flatten_to_rgb_aos(const unsigned char* pixels, int n_rows, int n_cols);
