#include "rgbsoa.h"

void flatten_to_rgb_soa_impl(const unsigned char* pixels, RGBSoA& soa,
                             int q_i, int q_j, int q_n_rows, int q_n_cols,
                             int idx, int N_COLS) {
    if (q_n_cols == 1) {
        soa.r(idx) = pixels[(q_i * N_COLS + q_j) * 3 + 0];
        soa.g(idx) = pixels[(q_i * N_COLS + q_j) * 3 + 1];
        soa.b(idx) = pixels[(q_i * N_COLS + q_j) * 3 + 2];
    } else {
        int subq_n_rows = q_n_rows / 2;
        int subq_n_cols = q_n_cols / 2;
        int subq_n_pixels = subq_n_rows * subq_n_cols;
        // clang-format off
        flatten_to_rgb_soa_impl(pixels, soa, q_i + 0,           q_j + 0,           subq_n_rows, subq_n_cols, idx + subq_n_pixels * 0, N_COLS); // nw
        flatten_to_rgb_soa_impl(pixels, soa, q_i + 0,           q_j + subq_n_cols, subq_n_rows, subq_n_cols, idx + subq_n_pixels * 1, N_COLS); // ne
        flatten_to_rgb_soa_impl(pixels, soa, q_i + subq_n_rows, q_j + subq_n_cols, subq_n_rows, subq_n_cols, idx + subq_n_pixels * 2, N_COLS); // se
        flatten_to_rgb_soa_impl(pixels, soa, q_i + subq_n_rows, q_j + 0,           subq_n_rows, subq_n_cols, idx + subq_n_pixels * 3, N_COLS); // sw
        // clang-format on
    }
}

RGBSoA flatten_to_rgb_soa(const unsigned char* pixels, int n_rows, int n_cols) {
    const int n_pixels = n_rows * n_cols;
    auto soa = RGBSoA{n_pixels, EigenArray(n_pixels), EigenArray(n_pixels), EigenArray(n_pixels)};
    flatten_to_rgb_soa_impl(pixels, soa, 0, 0, n_rows, n_cols, 0, n_cols);
    return soa;
}
