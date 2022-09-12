#include "bottom_up.h"
#include "quadrant.h"
#include "quadtree.h"

#include <future>          // PARALLEL
#include <spdlog/spdlog.h> // LOG_CONSTRUCTION

#include <cmath>

bool should_merge(float detail_threshold, const RGB<float>& std) {
    return std.r <= detail_threshold &&
           std.g <= detail_threshold &&
           std.b <= detail_threshold;
}

// source
// https://stats.stackexchange.com/questions/25848/how-to-sum-a-standard-deviation/442050#442050
RGB<float> combine_means(const Quadtree& nw, const Quadtree& ne, const Quadtree& se, const Quadtree& sw) {
    auto nw_mean = nw.mean(),
         ne_mean = ne.mean(),
         se_mean = se.mean(),
         sw_mean = sw.mean();
    return {
        (nw_mean.r + ne_mean.r + se_mean.r + sw_mean.r) / 4,
        (nw_mean.g + ne_mean.g + se_mean.g + sw_mean.g) / 4,
        (nw_mean.b + ne_mean.b + se_mean.b + sw_mean.b) / 4,
    };
}

RGB<float> combine_stds(const Quadtree& nw, const Quadtree& ne, const Quadtree& se, const Quadtree& sw, const RGB<float>& mean) {
    RGB<float> nw_mean = nw.mean(),
               ne_mean = ne.mean(),
               se_mean = se.mean(),
               sw_mean = sw.mean();
    RGB<float> nw_std = nw.std(),
               ne_std = ne.std(),
               se_std = se.std(),
               sw_std = sw.std();

    auto combine = [](float nw_std, float ne_std, float se_std, float sw_std,
                      float nw_mean, float ne_mean, float se_mean, float sw_mean,
                      float mean) {
        return sqrtf(
                   square(nw_std) + square(mean - nw_mean) +
                   square(ne_std) + square(mean - ne_mean) +
                   square(se_std) + square(mean - se_mean) +
                   square(sw_std) + square(mean - sw_mean)) /
               2;
    };

    return {
        combine(nw_std.r, ne_std.r, se_std.r, sw_std.r, nw_mean.r, ne_mean.r, se_mean.r, sw_mean.r, mean.r),
        combine(nw_std.g, ne_std.g, se_std.g, sw_std.g, nw_mean.g, ne_mean.g, se_mean.g, sw_mean.g, mean.g),
        combine(nw_std.b, ne_std.b, se_std.b, sw_std.b, nw_mean.b, ne_mean.b, se_mean.b, sw_mean.b, mean.b),
    };
}

std::unique_ptr<Quadtree> bottom_up_impl(std::unique_ptr<Quadrant> quadrant, float detail_threshold, int depth) {
    assert(detail_threshold >= 0);

    if (quadrant->n_cols == 1) { // A leaf has been reached
        const auto mean = quadrant->mean();
#ifdef LOG_CONSTRUCTION
        spdlog::debug("mean: {}/{}/{}, ", +mean.r, +mean.g, +mean.b);
#endif

        const auto std = RGB<float>{0, 0, 0};
#ifdef LOG_CONSTRUCTION
        spdlog::debug("std: {}/{}/{}, ", std.r, std.g, std.b);
#endif
        return std::make_unique<Quadtree>(quadrant->i, quadrant->j,
                                          quadrant->n_rows, quadrant->n_cols,
                                          Quadtree::Leaf{},
                                          mean, std);
    } else {
        std::unique_ptr<Quadtree> nw, ne, se, sw;
#ifndef NPARALLEL
        if (depth <= 1) { // Don't overload the system.
            // Spawn 3 threads, and assign 1 subquadrant to each of them.
            auto nwf = std::async(std::launch::async, bottom_up_impl, quadrant->nw(), detail_threshold, depth + 1);
            auto nef = std::async(std::launch::async, bottom_up_impl, quadrant->ne(), detail_threshold, depth + 1);
            auto sef = std::async(std::launch::async, bottom_up_impl, quadrant->se(), detail_threshold, depth + 1);
            nw = nwf.get();
            ne = nef.get();
            se = sef.get();
            // Assign the remaining subquadrant to the current thread.
            sw = bottom_up_impl(quadrant->sw(), detail_threshold, depth + 1);
        } else {
            nw = bottom_up_impl(quadrant->nw(), detail_threshold, depth + 1);
            ne = bottom_up_impl(quadrant->ne(), detail_threshold, depth + 1);
            se = bottom_up_impl(quadrant->se(), detail_threshold, depth + 1);
            sw = bottom_up_impl(quadrant->sw(), detail_threshold, depth + 1);
        }
#else
        nw = bottom_up_impl(quadrant->nw(), detail_threshold, depth + 1);
        ne = bottom_up_impl(quadrant->ne(), detail_threshold, depth + 1);
        se = bottom_up_impl(quadrant->se(), detail_threshold, depth + 1);
        sw = bottom_up_impl(quadrant->sw(), detail_threshold, depth + 1);
#endif
        auto mean = combine_means(*nw, *ne, *se, *sw);
        auto std = combine_stds(*nw, *ne, *se, *sw, mean);

        if (should_merge(detail_threshold, std)) {
#ifdef LOG_CONSTRUCTION
            spdlog::debug("leaf reached");
#endif
            return std::make_unique<Quadtree>(quadrant->i, quadrant->j,
                                              quadrant->n_rows, quadrant->n_cols,
                                              Quadtree::Leaf{},
                                              mean, std);
        } else {
#ifdef LOG_CONSTRUCTION
            spdlog::debug("should split");
#endif
            return std::make_unique<Quadtree>(quadrant->i, quadrant->j,
                                              quadrant->n_rows, quadrant->n_cols,
                                              Quadtree::Fork{std::move(nw), std::move(ne), std::move(se), std::move(sw)},
                                              mean, std);
        }
    }
}

std::unique_ptr<Quadtree> bottom_up(std::unique_ptr<Quadrant> quadrant, float detail_threshold) {
    return bottom_up_impl(std::move(quadrant), detail_threshold, 0);
}
