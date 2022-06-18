#ifndef ACA_FLATTEN_DATA_H
#define ACA_FLATTEN_DATA_H

#include "quadtree_with_eigen.h"

void flatten_data(uint8_t* data,
                  ColorVec& r, ColorVec& g, ColorVec& b,
                  unsigned i, unsigned j,
                  unsigned n_rows, unsigned n_cols,
                  unsigned idx,
                  unsigned N_COLS) {
    if (n_cols == 1) {
        r(idx) = data[(i * N_COLS + j) * 3 + 0];
        g(idx) = data[(i * N_COLS + j) * 3 + 1];
        b(idx) = data[(i * N_COLS + j) * 3 + 2];
    } else {
        auto q_rows = n_rows / 2,
             q_cols = n_cols / 2;
        auto q_size = q_rows * q_cols;
        // clang-format off
        flatten_data(data, r, g, b, i + 0,      j + 0,      q_rows, q_cols, idx + q_size * 0, N_COLS); // nw
        flatten_data(data, r, g, b, i + 0,      j + q_cols, q_rows, q_cols, idx + q_size * 1, N_COLS); // ne
        flatten_data(data, r, g, b, i + q_rows, j + 0,      q_rows, q_cols, idx + q_size * 2, N_COLS); // sw
        flatten_data(data, r, g, b, i + q_rows, j + q_cols, q_rows, q_cols, idx + q_size * 3, N_COLS); // se
        // clang-format on
    }
}

#endif // ACA_FLATTEN_DATA_H
