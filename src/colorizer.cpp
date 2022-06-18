#include <iostream>
#include <variant>

#include "colorizer.h"
#include "overloaded.h"

void colorize(uint8_t* data, const Quadtree& quadtree) {
    auto visit_fork = [&](const Quadtree::Fork& fork) {
        colorize(data, *fork.nw);
        colorize(data, *fork.ne);
        colorize(data, *fork.se);
        colorize(data, *fork.sw);
    };
    auto visit_leaf = [&](const Quadtree::Leaf& leaf) {
        const auto i_from = quadtree.i;
        const auto j_from = quadtree.j;

        const auto i_to = i_from + quadtree.n_rows;
        const auto j_to = j_from + quadtree.n_cols;

        for (auto i = i_from; i < i_to; i++) {
            for (auto j = j_from; j < j_to; j++) {
                data[(i * quadtree.n_cols + j) * 3 + 0] = leaf.r;
                data[(i * quadtree.n_cols + j) * 3 + 1] = leaf.g;
                data[(i * quadtree.n_cols + j) * 3 + 2] = leaf.b;
            }
        }
    };
    auto visit_empty = [](const Quadtree::Empty&) {};

    std::visit(overloaded{visit_fork, visit_leaf, visit_empty}, quadtree.data);
}
