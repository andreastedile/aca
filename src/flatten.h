#ifndef ACA_FLATTEN_DATA_H
#define ACA_FLATTEN_DATA_H

#include "quadtree_with_eigen.h"

void flatten_data(uint8_t* data,
                  ColorVec& r, ColorVec& g, ColorVec& b,
                  int i, int j,
                  int n_rows, int n_cols,
                  int& idx,
                  int N_COLS) {
    if (n_cols == 1) {
        r(idx) = data[(i * N_COLS + j) * 3 + 0];
        g(idx) = data[(i * N_COLS + j) * 3 + 1];
        b(idx) = data[(i * N_COLS + j) * 3 + 2];
        idx++;
    } else {
        // clang-format off
        flatten_data(data, r, g, b, i + 0,          j + 0,          n_rows / 2, n_cols / 2, idx, N_COLS); // nw
        flatten_data(data, r, g, b, i + 0,          j + n_cols / 2, n_rows / 2, n_cols / 2, idx, N_COLS); // ne
        flatten_data(data, r, g, b, i + n_rows / 2, j + 0,          n_rows / 2, n_cols / 2, idx, N_COLS); // sw
        flatten_data(data, r, g, b, i + n_rows / 2, j + n_cols / 2, n_rows / 2, n_cols / 2, idx, N_COLS); // se
        // clang-format on
    }
}

#endif // ACA_FLATTEN_DATA_H
