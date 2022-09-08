#ifndef ACA_TOP_DOWN_H
#define ACA_TOP_DOWN_H

#include "quadrant.h"
#include "quadtree.h"

#ifdef PARALLEL
#include <future>
#endif
#include <memory>
#ifdef LOG_QUADTREE_BUILD
#include <iostream>
#endif
#include <cmath>
#include <string>

bool should_split(double detail_threshold, int height, const RGB<double>& std) {
    return height > 0 &&
           std.r > detail_threshold &&
           std.g > detail_threshold &&
           std.b > detail_threshold;
}

std::unique_ptr<Quadtree> top_down_impl(std::unique_ptr<Quadrant> quadrant, double detail_threshold,
                                        int height, int depth) {
    assert(detail_threshold >= 0);
    assert(height >= 0);

    const auto mean = quadrant->mean();
#ifdef LOG_QUADTREE_BUILD
    std::cout << std::string(depth * 4, ' ') << "mean: " << +mean.r << ' ' << +mean.g << ' ' << +mean.b << '\n';
#endif

    const auto sq_mean = quadrant->sq_mean();
#ifdef LOG_QUADTREE_BUILD
    std::cout << std::string(depth * 4, ' ') << "sq_mean: " << sq_mean.r << ' ' << sq_mean.g << ' ' << sq_mean.b << '\n';
#endif

    const auto std = RGB<double>{
        std::sqrt(sq_mean.r - std::pow(mean.r, 2)),
        std::sqrt(sq_mean.g - std::pow(mean.g, 2)),
        std::sqrt(sq_mean.b - std::pow(mean.b, 2))};
#ifdef LOG_QUADTREE_BUILD
    std::cout << std::string(depth * 4, ' ') << "std: " << std.r << ' ' << std.g << ' ' << std.b << '\n';
#endif

    if (should_split(detail_threshold, height, std)) {
#ifdef LOG_QUADTREE_BUILD
        std::cout << std::string(depth * 4, ' ') << "split\n";
#endif

        std::unique_ptr<Quadtree> nw, ne, se, sw;
#ifdef PARALLEL
        if (depth <= 1) { // Don't overload the system.
            // Spawn 3 threads, and assign 1 subquadrant to each of them.
            auto nwf = std::async(std::launch::async, top_down_impl, quadrant->nw(), detail_threshold, height - 1, depth + 1);
            auto nef = std::async(std::launch::async, top_down_impl, quadrant->ne(), detail_threshold, height - 1, depth + 1);
            auto sef = std::async(std::launch::async, top_down_impl, quadrant->se(), detail_threshold, height - 1, depth + 1);
            nw = nwf.get();
            ne = nef.get();
            se = sef.get();
            // Assign the remaining subquadrant to the current thread.
            sw = top_down_impl(quadrant->sw(), detail_threshold, height - 1, depth + 1);
        } else {
            nw = top_down_impl(quadrant->nw(), detail_threshold, height - 1, depth + 1);
            ne = top_down_impl(quadrant->ne(), detail_threshold, height - 1, depth + 1);
            se = top_down_impl(quadrant->se(), detail_threshold, height - 1, depth + 1);
            sw = top_down_impl(quadrant->sw(), detail_threshold, height - 1, depth + 1);
        }
#else
        nw = top_down_impl(quadrant->nw(), detail_threshold, height - 1, depth + 1);
        ne = top_down_impl(quadrant->ne(), detail_threshold, height - 1, depth + 1);
        se = top_down_impl(quadrant->se(), detail_threshold, height - 1, depth + 1);
        sw = top_down_impl(quadrant->sw(), detail_threshold, height - 1, depth + 1);
#endif
        return std::make_unique<Quadtree>(height, depth,
                                          quadrant->i, quadrant->j,
                                          quadrant->n_rows, quadrant->n_cols,
                                          Quadtree::Fork{std::move(nw), std::move(ne), std::move(sw), std::move(se)},
                                          mean, std);
    } else {
#ifdef LOG_QUADTREE_BUILD
        std::cout << std::string(depth * 4, ' ') << "leaf\n";
#endif
        return std::make_unique<Quadtree>(height, depth,
                                          quadrant->i, quadrant->j,
                                          quadrant->n_rows, quadrant->n_cols,
                                          Quadtree::Leaf{},
                                          mean, std);
    }
}

std::unique_ptr<Quadtree> top_down(std::unique_ptr<Quadrant> quadrant, double detail_threshold, int max_depth) {
    return top_down_impl(std::move(quadrant), detail_threshold, max_depth, 0);
}

#endif // ACA_TOP_DOWN_H
