#include <cassert>
#include <variant>

#include "../utils/overloaded.h"
#include "colorizer.h"

void colorize_impl(unsigned char* pixels, int N_COLS, const Quadtree& quadtree) {
    auto visit_fork = [&](const Quadtree::Fork& fork) {
        colorize_impl(pixels, N_COLS, *fork.nw);
        colorize_impl(pixels, N_COLS, *fork.ne);
        colorize_impl(pixels, N_COLS, *fork.se);
        colorize_impl(pixels, N_COLS, *fork.sw);
    };
    auto visit_leaf = [&](const Quadtree::Leaf& leaf) {
        const auto i_from = quadtree.i;
        const auto j_from = quadtree.j;

        const auto i_to = i_from + quadtree.n_rows;
        const auto j_to = j_from + quadtree.n_cols;

        for (auto i = i_from; i < i_to; i++) {
            for (auto j = j_from; j < j_to; j++) {
                pixels[(i * N_COLS + j) * 3 + 0] = quadtree.color().r;
                pixels[(i * N_COLS + j) * 3 + 1] = quadtree.color().g;
                pixels[(i * N_COLS + j) * 3 + 2] = quadtree.color().b;
            }
        }
    };

    std::visit(overloaded{visit_fork, visit_leaf}, quadtree.data());
}

void colorize(unsigned char* pixels, int n_rows, int n_cols, const Quadtree& quadtree) {
    assert(n_rows * n_cols == quadtree.n_rows * quadtree.n_cols);
    colorize_impl(pixels, n_cols, quadtree);
}