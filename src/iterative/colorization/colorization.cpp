#include "colorization.h"

void colorize_impl(unsigned char* pixels, int N_COLS, const Node* quadtree_array, int node_idx, int q_i, int q_j, int q_n_rows, int q_n_cols, int n_levels_to_skip) {
    if (n_levels_to_skip <= 0 && quadtree_array[node_idx].type == Node::Type::LEAF) {
        const int i_from = q_i;
        const int j_from = q_j;

        const int i_to = i_from + q_n_rows;
        const int j_to = j_from + q_n_cols;

        for (int i = i_from; i < i_to; i++) {
            for (auto j = j_from; j < j_to; j++) {
                pixels[(i * N_COLS + j) * 3 + 0] = quadtree_array[node_idx].color().r;
                pixels[(i * N_COLS + j) * 3 + 1] = quadtree_array[node_idx].color().g;
                pixels[(i * N_COLS + j) * 3 + 2] = quadtree_array[node_idx].color().b;
            }
        }
    } else {
        // clang-format off
        colorize_impl(pixels, N_COLS, quadtree_array, 4 * node_idx + 1, q_i,                q_j,                q_n_rows / 2, q_n_cols / 2, n_levels_to_skip-1);// NW
        colorize_impl(pixels, N_COLS, quadtree_array, 4 * node_idx + 2, q_i,                q_j + q_n_cols / 2, q_n_rows / 2, q_n_cols / 2, n_levels_to_skip-1);// NE
        colorize_impl(pixels, N_COLS, quadtree_array, 4 * node_idx + 3, q_i + q_n_rows / 2, q_j + q_n_cols / 2, q_n_rows / 2, q_n_cols / 2, n_levels_to_skip-1);// SE
        colorize_impl(pixels, N_COLS, quadtree_array, 4 * node_idx + 4, q_i + q_n_rows / 2, q_j,                q_n_rows / 2, q_n_cols / 2, n_levels_to_skip-1);// SW
        // clang-format on
    }
}

void colorize(unsigned char* pixels, int n_rows, int n_cols, const Node* quadtree_array, int colorize_up_to_level) {
    colorize_impl(pixels, n_cols, quadtree_array, 0, 0, 0, n_rows, n_cols, colorize_up_to_level);
}
