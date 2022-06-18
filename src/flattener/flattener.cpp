#include "../quadtree_with_eigen.h" // RgbSoa
#include "flattener.h"

void flatten_data_impl(uint8_t* data,
                       RgbSoa& soa,
                       unsigned i, unsigned j,
                       unsigned n_rows, unsigned n_cols,
                       unsigned idx,
                       unsigned N_COLS) {
    if (n_cols == 1) {
        soa.r(idx) = data[(i * N_COLS + j) * 3 + 0];
        soa.g(idx) = data[(i * N_COLS + j) * 3 + 1];
        soa.b(idx) = data[(i * N_COLS + j) * 3 + 2];
    } else {
        auto q_rows = n_rows / 2,
             q_cols = n_cols / 2;
        auto q_size = q_rows * q_cols;
        // clang-format off
        flatten_data_impl(data, soa, i + 0,      j + 0,      q_rows, q_cols, idx + q_size * 0, N_COLS); // nw
        flatten_data_impl(data, soa, i + 0,      j + q_cols, q_rows, q_cols, idx + q_size * 1, N_COLS); // ne
        flatten_data_impl(data, soa, i + q_rows, j + 0,      q_rows, q_cols, idx + q_size * 2, N_COLS); // sw
        flatten_data_impl(data, soa, i + q_rows, j + q_cols, q_rows, q_cols, idx + q_size * 3, N_COLS); // se
        // clang-format on
    }
}

RgbSoa flatten_data(uint8_t* data, unsigned n_rows, unsigned n_cols) {
    RgbSoa soa{n_rows * n_cols};
    // Fixme parameter order in function call
    flatten_data_impl(data, soa, 0, 0, n_cols, n_rows, 0, n_cols);
    return soa;
}
