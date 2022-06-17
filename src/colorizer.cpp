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
        const auto i_to = quadtree.i + quadtree.n_rows;
        const auto j_to = quadtree.j + quadtree.n_cols;
        for (auto i = quadtree.i; i < i_to; i++) {
            for (auto j = quadtree.j; j < j_to; j++) {
                data[(i * quadtree.n_cols + j) * 3 + 0] = leaf.r;
                data[(i * quadtree.n_cols + j) * 3 + 1] = leaf.g;
                data[(i * quadtree.n_cols + j) * 3 + 2] = leaf.b;
            }
        }
    };
    auto visit_empty = [](const Quadtree::Empty&) {};

    std::visit(overloaded{visit_fork, visit_leaf, visit_empty}, quadtree.data);
}
