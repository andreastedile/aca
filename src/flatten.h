#ifndef ACA_FLATTEN_DATA_H
#define ACA_FLATTEN_DATA_H

#include "quadtree_with_eigen.h"

void flatten_data(uint8_t* data,
                  ColorVec& r, ColorVec& g, ColorVec& b,
                  int top, int left,
                  int w, int h,
                  int& i,
                  int W) {
    if (w == 1) {
        r(i) = data[(top * W + left) * 3 + 0];
        g(i) = data[(top * W + left) * 3 + 1];
        b(i) = data[(top * W + left) * 3 + 2];
        i++;
    } else {
        flatten_data(data, r, g, b, top, left, w / 2, h / 2, i, W);
        flatten_data(data, r, g, b, top, left + w / 2, w / 2, h / 2, i, W);
        flatten_data(data, r, g, b, top + h / 2, left, w / 2, h / 2, i, W);
        flatten_data(data, r, g, b, top + h / 2, left + w / 2, w / 2, h / 2, i, W);
    }
}

void unflatten_data(uint8_t* data,
                    ColorVec& r, ColorVec& g, ColorVec& b,
                    int top, int left,
                    int w, int h,
                    int& i,
                    int W) {
    if (w == 1) {
        data[(top * W + left) * 3 + 0] = r(i);
        data[(top * W + left) * 3 + 1] = g(i);
        data[(top * W + left) * 3 + 2] = b(i);
        i++;
    } else {
        unflatten_data(data, r, g, b, top, left, w / 2, h / 2, i, W);
        unflatten_data(data, r, g, b, top, left + w / 2, w / 2, h / 2, i, W);
        unflatten_data(data, r, g, b, top + h / 2, left, w / 2, h / 2, i, W);
        unflatten_data(data, r, g, b, top + h / 2, left + w / 2, w / 2, h / 2, i, W);
    }
}

#endif // ACA_FLATTEN_DATA_H
