#include "rgbsoa.h"

void flatten_to_rgb_soa_impl(const unsigned char* pixels,
                             RGBSoA& soa,
                             int i, int j,
                             int n_rows, int n_cols,
                             int idx,
                             int N_COLS) {
    if (n_cols == 1) {
        soa.r(idx) = pixels[(i * N_COLS + j) * 3 + 0];
        soa.g(idx) = pixels[(i * N_COLS + j) * 3 + 1];
        soa.b(idx) = pixels[(i * N_COLS + j) * 3 + 2];
    } else {
        auto q_rows = n_rows / 2,
             q_cols = n_cols / 2;
        auto q_size = q_rows * q_cols;
        // clang-format off
        flatten_to_rgb_soa_impl(pixels, soa, i + 0,      j + 0,      q_rows, q_cols, idx + q_size * 0, N_COLS); // nw
        flatten_to_rgb_soa_impl(pixels, soa, i + 0,      j + q_cols, q_rows, q_cols, idx + q_size * 1, N_COLS); // ne
        flatten_to_rgb_soa_impl(pixels, soa, i + q_rows, j + 0,      q_rows, q_cols, idx + q_size * 2, N_COLS); // sw
        flatten_to_rgb_soa_impl(pixels, soa, i + q_rows, j + q_cols, q_rows, q_cols, idx + q_size * 3, N_COLS); // se
        // clang-format on
    }
}

RGBSoA flatten_to_rgb_soa(const unsigned char* pixels, int n_pixels) {
    auto soa = RGBSoA{EigenArray(n_pixels), EigenArray(n_pixels), EigenArray(n_pixels)};
    // Fixme: fix the order of the parameters in the function call
    flatten_to_rgb_soa_impl(pixels, soa, 0, 0, std::sqrt(n_pixels), std::sqrt(n_pixels), 0, std::sqrt(n_pixels));
    return soa;
}
