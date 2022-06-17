#ifndef ACA_FLATTEN_DATA_H
#define ACA_FLATTEN_DATA_H

#include "quadtree_with_eigen.h"

void flatten_data(uint8_t* data,
                  ColorVec& r, ColorVec& g, ColorVec& b,
                  int x, int y,
                  int h, int w,
                  int& i,
                  int W) {
    if (w == 1) {
        r(i) = data[(x * W + y) * 3 + 0];
        g(i) = data[(x * W + y) * 3 + 1];
        b(i) = data[(x * W + y) * 3 + 2];
        i++;
    } else {
        // clang-format off
        flatten_data(data, r, g, b, x +     0, y +     0, h / 2, w / 2, i, W); // nw
        flatten_data(data, r, g, b, x +     0, y + w / 2, h / 2, w / 2, i, W); // ne
        flatten_data(data, r, g, b, x + h / 2, y +     0, h / 2, w / 2, i, W); // sw
        flatten_data(data, r, g, b, x + h / 2, y + w / 2, h / 2, w / 2, i, W); // se
        // clang-format on
    }
}

#endif // ACA_FLATTEN_DATA_H
