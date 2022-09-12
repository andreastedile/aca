#include "top_down.h"
#include "quadrant.h"
#include "quadtree.h"

#include <future>
#include <spdlog/spdlog.h> // LOG_CONSTRUCTION

#include <cmath>

bool should_split(float detail_threshold, const RGB<float>& std) {
    return std.r > detail_threshold ||
           std.g > detail_threshold ||
           std.b > detail_threshold;
}

std::unique_ptr<QtNode> top_down_impl(std::unique_ptr<Extents> quadrant, float detail_threshold, int depth) {
    assert(detail_threshold >= 0);

    const auto mean = quadrant->mean();
#ifdef LOG_CONSTRUCTION
    spdlog::debug("mean: {}/{}/{}, ", +mean.r, +mean.g, +mean.b);
#endif

    const auto sq_mean = quadrant->sq_mean();
#ifdef LOG_CONSTRUCTION
    spdlog::debug("sq mean: {}/{}/{}, ", +sq_mean.r, +sq_mean.g, +sq_mean.b);
#endif

    const auto std = RGB<float>{
        sqrtf(sq_mean.r - powf(mean.r, 2)),
        sqrtf(sq_mean.g - powf(mean.g, 2)),
        sqrtf(sq_mean.b - powf(mean.b, 2))};
#ifdef LOG_CONSTRUCTION
    spdlog::debug("std: {}/{}/{}, ", std.r, std.g, std.b);
#endif

    if (should_split(detail_threshold, std)) { // this condition is also verified when a leaf is reached
#ifdef LOG_CONSTRUCTION
        spdlog::debug("should split");
#endif

        std::unique_ptr<QtNode> nw, ne, se, sw;
#ifndef NPARALLEL
        if (depth <= 1) { // Don't overload the system.
            // Spawn 3 threads, and assign 1 subquadrant to each of them.
            auto nwf = std::async(std::launch::async, top_down_impl, quadrant->nw(), detail_threshold, depth + 1);
            auto nef = std::async(std::launch::async, top_down_impl, quadrant->ne(), detail_threshold, depth + 1);
            auto sef = std::async(std::launch::async, top_down_impl, quadrant->se(), detail_threshold, depth + 1);
            nw = nwf.get();
            ne = nef.get();
            se = sef.get();
            // Assign the remaining subquadrant to the current thread.
            sw = top_down_impl(quadrant->sw(), detail_threshold, depth + 1);
        } else {
            nw = top_down_impl(quadrant->nw(), detail_threshold, depth + 1);
            ne = top_down_impl(quadrant->ne(), detail_threshold, depth + 1);
            se = top_down_impl(quadrant->se(), detail_threshold, depth + 1);
            sw = top_down_impl(quadrant->sw(), detail_threshold, depth + 1);
        }
#else
        nw = top_down_impl(quadrant->nw(), detail_threshold, depth + 1);
        ne = top_down_impl(quadrant->ne(), detail_threshold, depth + 1);
        se = top_down_impl(quadrant->se(), detail_threshold, depth + 1);
        sw = top_down_impl(quadrant->sw(), detail_threshold, depth + 1);
#endif
        return std::make_unique<QtNode>(quadrant->i, quadrant->j,
                                        quadrant->n_rows, quadrant->n_cols,
                                        QtNode::Fork{std::move(nw), std::move(ne), std::move(se), std::move(sw)},
                                        mean, std);
    } else {
#ifdef LOG_CONSTRUCTION
        spdlog::debug("leaf reached");
#endif
        return std::make_unique<QtNode>(quadrant->i, quadrant->j,
                                        quadrant->n_rows, quadrant->n_cols,
                                        QtNode::Leaf{},
                                        mean, std);
    }
}

std::unique_ptr<QtNode> top_down(std::unique_ptr<Extents> quadrant, float detail_threshold) {
    return top_down_impl(std::move(quadrant), detail_threshold, 0);
}
