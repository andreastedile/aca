#include <iostream>
#include "colorizer.h"

uint8_t *colorize(Quadtree &quadtree) {
    int w = quadtree.w;
    int h = quadtree.h;
    uint8_t *data = new uint8_t[w * h * 3];
    auto leaves = quadtree.get_leaves();
    std::for_each(leaves.begin(), leaves.end(), [&](Quadtree::Leaf &l) {
        int mx = l.x + l.h; // max x
        int my = l.y + l.w; // max y
        for (int i = l.x; i < mx; i++) {
            for (int j = l.y; j < my; j++) {
                data[(i * w + j) * 3 + 0] = l.r;
                data[(i * w + j) * 3 + 1] = l.g;
                data[(i * w + j) * 3 + 2] = l.b;
            }
        }
    });
    return data;
}
