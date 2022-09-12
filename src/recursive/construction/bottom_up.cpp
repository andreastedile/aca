#include "bottom_up.h"
#include "../overloaded.h"
#include "aos_ops.h"
#include "qtmath.h"
#include "quadrant.h"
#include "quadtree.h"
#include "rgbaos.h"

#include <future>          // PARALLEL
#include <spdlog/spdlog.h> // LOG_CONSTRUCTION

#include <cmath>

// source
// https://stats.stackexchange.com/questions/25848/how-to-sum-a-standard-deviation/442050#442050
RGB<float> combine_means(const QtNode& nw, const QtNode& ne, const QtNode& se, const QtNode& sw) {
    auto nw_mean = mean_of(nw);
    auto ne_mean = mean_of(ne);
    auto se_mean = mean_of(se);
    auto sw_mean = mean_of(sw);
    return {
        (nw_mean.r + ne_mean.r + se_mean.r + sw_mean.r) / 4,
        (nw_mean.g + ne_mean.g + se_mean.g + sw_mean.g) / 4,
        (nw_mean.b + ne_mean.b + se_mean.b + sw_mean.b) / 4,
    };
}

RGB<float> combine_stds(const QtNode& nw, const QtNode& ne, const QtNode& se, const QtNode& sw, const RGB<float>& mean) {
    auto nw_mean = mean_of(nw);
    auto ne_mean = mean_of(ne);
    auto se_mean = mean_of(se);
    auto sw_mean = mean_of(sw);

    auto nw_std = std_of(nw);
    auto ne_std = std_of(ne);
    auto se_std = std_of(se);
    auto sw_std = std_of(sw);

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

std::unique_ptr<QtNode> bottom_up_impl(const RGBAoS& aos, const Subquadrant& quadrant, float detail_threshold, int depth) {
    assert(detail_threshold >= 0);

    const auto visit_extents = [&](const Extents& visited) -> std::unique_ptr<QtNode> {
        std::unique_ptr<QtNode> nw, ne, se, sw;

#ifndef NPARALLEL
        if (depth <= 1) { // Don't overload the system.
            // Spawn 3 threads, and assign 1 subquadrant to each of them.
            auto nwf = std::async(std::launch::async, bottom_up_impl, aos, nw_subquadrant(visited), detail_threshold, depth + 1);
            auto nef = std::async(std::launch::async, bottom_up_impl, aos, ne_subquadrant(visited), detail_threshold, depth + 1);
            auto sef = std::async(std::launch::async, bottom_up_impl, aos, se_subquadrant(visited), detail_threshold, depth + 1);
            nw = nwf.get();
            ne = nef.get();
            se = sef.get();
            // Assign the remaining subquadrant to the current thread.
            sw = bottom_up_impl(aos, sw_subquadrant(visited), detail_threshold, depth + 1);
        } else {
            nw = bottom_up_impl(aos, nw_subquadrant(visited), detail_threshold, depth + 1);
            ne = bottom_up_impl(aos, ne_subquadrant(visited), detail_threshold, depth + 1);
            se = bottom_up_impl(aos, se_subquadrant(visited), detail_threshold, depth + 1);
            sw = bottom_up_impl(aos, sw_subquadrant(visited), detail_threshold, depth + 1);
        }
#else
        nw = bottom_up_impl(aos, nw_subquadrant(extents), detail_threshold, depth + 1);
        ne = bottom_up_impl(aos, ne_subquadrant(extents), detail_threshold, depth + 1);
        se = bottom_up_impl(aos, se_subquadrant(extents), detail_threshold, depth + 1);
        sw = bottom_up_impl(aos, sw_subquadrant(extents), detail_threshold, depth + 1);
#endif
        auto mean = combine_means(*nw, *ne, *se, *sw);
        auto std = combine_stds(*nw, *ne, *se, *sw, mean);

        if (should_merge(detail_threshold, std)) {
#ifdef LOG_CONSTRUCTION
            spdlog::debug("leaf reached");
#endif
            auto leaf = Leaf{visited.top_left, RGB<unsigned char>((unsigned char)mean.r, (unsigned char)mean.g, (unsigned char)mean.b)};
            return std::make_unique<QtNode>(leaf);
        } else {
#ifdef LOG_CONSTRUCTION
            spdlog::debug("should split");
#endif
            return std::make_unique<QtNode>(Fork{std::move(nw), std::move(ne), std::move(se), std::move(sw), visited, mean, std});
        }
    };

    const auto visit_pixel = [&](const Position& position) -> std::unique_ptr<QtNode> {
#ifdef LOG_CONSTRUCTION
        spdlog::debug("mean: {}/{}/{}, ", +mean.r, +mean.g, +mean.b);
#endif

        const auto std = RGB<float>{0, 0, 0};
#ifdef LOG_CONSTRUCTION
        spdlog::debug("std: {}/{}/{}, ", std.r, std.g, std.b);
#endif
        auto pixel = compute_pixel(aos, position.i, position.j);
        auto leaf = Leaf{position, RGB<unsigned char>(pixel.r, pixel.g, pixel.b)};
        return std::make_unique<QtNode>(leaf);
    };

    return std::visit(overloaded{visit_extents, visit_pixel}, quadrant);
}

std::unique_ptr<QtNode> bottom_up(const RGBAoS& aos, float detail_threshold) {
    return bottom_up_impl(aos, Extents{0, 0, (int)sqrt(aos.n_elements)}, detail_threshold, 0);
}
