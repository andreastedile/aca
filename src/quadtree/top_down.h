#ifndef ACA_TOP_DOWN_H
#define ACA_TOP_DOWN_H

#include "quadrant.h"
#include "quadtree.h"

#include <memory>
#ifdef LOG_QUADTREE_BUILD
#include <iostream>
#endif
#include <cmath>
#include <string>

bool should_split(double detail_threshold, int height, const RGB<double>& stdev) {
    return height > 0 &&
           stdev.r > detail_threshold &&
           stdev.g > detail_threshold &&
           stdev.b > detail_threshold;
}

std::unique_ptr<Quadtree> top_down_impl(const Quadrant& quadrant, double detail_threshold,
                                        int height, int depth) {
    assert(detail_threshold >= 0);
    assert(height >= 0);
    assert(i >= 0);
    assert(j >= 0);
    assert(n_rows >= 1);
    assert(n_cols >= 1);

    const auto mean = quadrant.mean();
#ifdef LOG_QUADTREE_BUILD
    std::cout << std::string(depth * 4, ' ') << "mean: " << +mean.r << ' ' << +mean.g << ' ' << +mean.b << '\n';
#endif

    const auto sq_mean = quadrant.sq_mean();
#ifdef LOG_QUADTREE_BUILD
    std::cout << std::string(depth * 4, ' ') << "sq_mean: " << sq_mean.r << ' ' << sq_mean.g << ' ' << sq_mean.b << '\n';
#endif

    const auto stdev = RGB<double>{
        std::sqrt(sq_mean.r - std::pow(mean.r, 2)),
        std::sqrt(sq_mean.g - std::pow(mean.g, 2)),
        std::sqrt(sq_mean.b - std::pow(mean.b, 2))};
#ifdef LOG_QUADTREE_BUILD
    std::cout << std::string(depth * 4, ' ') << "stdev: " << stdev.r << ' ' << stdev.g << ' ' << stdev.b << '\n';
#endif

    if (should_split(detail_threshold, height, stdev)) {
#ifdef LOG_QUADTREE_BUILD
        std::cout << std::string(depth * 4, ' ') << "split\n";
#endif

        // clang-format off
        auto nw = top_down_impl(*quadrant.nw(), detail_threshold, height - 1, depth + 1);
        auto ne = top_down_impl(*quadrant.ne(), detail_threshold, height - 1, depth + 1);
        auto se = top_down_impl(*quadrant.sw(), detail_threshold, height - 1, depth + 1);
        auto sw = top_down_impl(*quadrant.se(), detail_threshold, height - 1, depth + 1);
        // clang-format on

        return std::make_unique<Quadtree>(height, depth,
                                          quadrant.i, quadrant.j,
                                          quadrant.n_rows, quadrant.n_cols,
                                          Quadtree::Fork{std::move(nw), std::move(ne), std::move(sw), std::move(se)},
                                          mean);
    } else {
#ifdef LOG_QUADTREE_BUILD
        std::cout << std::string(depth * 4, ' ') << "leaf\n";
#endif
        return std::make_unique<Quadtree>(height, depth,
                                          quadrant.i, quadrant.j,
                                          quadrant.n_rows, quadrant.n_cols,
                                          Quadtree::Leaf{},
                                          mean);
    }
}

std::unique_ptr<Quadtree> top_down(const Quadrant& quadrant, double detail_threshold, int max_depth) {
    return top_down_impl(quadrant, detail_threshold, max_depth, 0);
}

#endif // ACA_TOP_DOWN_H
