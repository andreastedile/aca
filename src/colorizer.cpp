#include <iostream>
#include "colorizer.h"

uint8_t *colorize(Quadtree &quadtree) {
    int n_cols = quadtree.n_cols;
    int n_rows = quadtree.n_rows;
    uint8_t *data = new uint8_t[n_cols * n_rows * 3];
    auto leaves = quadtree.get_leaves();
    std::for_each(leaves.begin(), leaves.end(), [&](Quadtree::Leaf &l) {
        int mx = l.i + l.n_rows; // max x
        int my = l.j + l.n_cols; // max y
        for (int i = l.i; i < mx; i++) {
            for (int j = l.j; j < my; j++) {
                data[(i * n_cols + j) * 3 + 0] = l.r;
                data[(i * n_cols + j) * 3 + 1] = l.g;
                data[(i * n_cols + j) * 3 + 2] = l.b;
            }
        }
    });
    return data;
}
