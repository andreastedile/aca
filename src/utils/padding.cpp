#include "padding.h"

int get_closest_power_of_two(int n, int m) {
    int pow = 1;
    while (pow < n || pow < m) {
        pow *= 2;
    }
    return pow;
}

bool needs_padding(int n_rows, int n_cols) {
    return ((n_rows & (n_rows - 1)) != 0) || ((n_cols & (n_cols - 1)) != 0) || (n_rows != n_cols);
}

std::optional<unsigned char*> pad_image(const unsigned char* pixels, const int n_rows, const int n_cols, int& new_rows, int& new_cols) {
    if (!needs_padding(n_rows, n_cols)) {
        new_rows = n_rows;
        new_cols = n_cols;
        return {};
    }

    new_rows = new_cols = get_closest_power_of_two(n_rows, n_cols);

    unsigned char* new_pixels = new unsigned char[new_rows * new_cols * 3]{};

    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; j < n_cols; j++) {
            for (int k = 0; k < 3; k++) {
                new_pixels[3 * (i * new_cols + j) + k] = pixels[3 * (i * n_cols + j) + k];
            }
        }
    }

    return {new_pixels};
}
