#ifndef BOTTOM_UP_H
#define BOTTOM_UP_H

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

bool should_merge(double detail_threshold, int height, const RGB<double>& std) {
    return std.r <= detail_threshold ||
           std.g <= detail_threshold ||
           std.b <= detail_threshold;
}

// source
// https://stats.stackexchange.com/questions/25848/how-to-sum-a-standard-deviation/442050#442050
RGB<double> combine_means(const Quadtree& nw, const Quadtree& ne, const Quadtree& se, const Quadtree& sw) {
    int pixels = nw.n_rows * nw.n_cols + ne.n_rows * ne.n_cols + se.n_rows * se.n_cols + sw.n_rows * sw.n_cols;
    auto nw_mean = nw.mean(),
         ne_mean = ne.mean(),
         se_mean = se.mean(),
         sw_mean = sw.mean();
    auto nw_pixels = nw.n_rows * nw.n_cols,
         ne_pixels = ne.n_rows * ne.n_cols,
         se_pixels = se.n_rows * se.n_cols,
         sw_pixels = sw.n_rows * nw.n_cols;
    return {
        (nw_mean.r * nw_pixels + ne_mean.r * ne_pixels + se_mean.r * se_pixels + sw_mean.r * sw_pixels) / pixels,
        (nw_mean.g * nw_pixels + ne_mean.g * ne_pixels + se_mean.g * se_pixels + sw_mean.g * sw_pixels) / pixels,
        (nw_mean.b * nw_pixels + ne_mean.b * ne_pixels + se_mean.b * se_pixels + sw_mean.b * sw_pixels) / pixels,
    };
}

RGB<double> combine_stds(const Quadtree& nw, const Quadtree& ne, const Quadtree& se, const Quadtree& sw, const RGB<double>& mean) {
    int pixels = nw.n_rows * nw.n_cols + ne.n_rows * ne.n_cols + se.n_rows * se.n_cols + sw.n_rows * sw.n_cols;
    auto nw_mean = nw.mean(),
         ne_mean = ne.mean(),
         se_mean = se.mean(),
         sw_mean = sw.mean();
    auto nw_std = nw.std(),
         ne_std = ne.std(),
         se_std = se.std(),
         sw_std = sw.std();
    auto nw_pixels = nw.n_rows * nw.n_cols,
         ne_pixels = ne.n_rows * ne.n_cols,
         se_pixels = se.n_rows * se.n_cols,
         sw_pixels = sw.n_rows * nw.n_cols;

    return {
        std::sqrt(
            (std::pow(nw_std.r, 2) * nw_pixels + nw_pixels * std::pow(mean.r - nw_mean.r, 2) +
             std::pow(ne_std.r, 2) * ne_pixels + ne_pixels * std::pow(mean.r - ne_mean.r, 2) +
             std::pow(se_std.r, 2) * se_pixels + se_pixels * std::pow(mean.r - se_mean.r, 2) +
             std::pow(sw_std.r, 2) * sw_pixels + sw_pixels * std::pow(mean.r - sw_mean.r, 2)) /
            pixels),

        std::sqrt(
            (std::pow(nw_std.g, 2) * nw_pixels + nw_pixels * std::pow(mean.g - nw_mean.g, 2) +
             std::pow(ne_std.g, 2) * ne_pixels + ne_pixels * std::pow(mean.g - ne_mean.g, 2) +
             std::pow(se_std.g, 2) * se_pixels + se_pixels * std::pow(mean.g - se_mean.g, 2) +
             std::pow(sw_std.g, 2) * sw_pixels + sw_pixels * std::pow(mean.g - sw_mean.g, 2)) /
            pixels),

        std::sqrt(
            (std::pow(nw_std.b, 2) * nw_pixels + nw_pixels * std::pow(mean.b - nw_mean.b, 2) +
             std::pow(ne_std.b, 2) * ne_pixels + ne_pixels * std::pow(mean.b - ne_mean.b, 2) +
             std::pow(se_std.b, 2) * se_pixels + se_pixels * std::pow(mean.b - se_mean.b, 2) +
             std::pow(sw_std.b, 2) * sw_pixels + sw_pixels * std::pow(mean.b - sw_mean.b, 2)) /
            pixels),
    };
}
std::unique_ptr<Quadtree> bottom_up_impl(std::unique_ptr<Quadrant> quadrant, double detail_threshold,
                                         int height, int depth) {
    assert(detail_threshold >= 0);
    assert(height >= 0);

    if (height == 0 || quadrant->n_cols == 1) {
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
        return std::make_unique<Quadtree>(height, depth,
                                          quadrant->i, quadrant->j,
                                          quadrant->n_rows, quadrant->n_cols,
                                          Quadtree::Leaf{},
                                          mean, std);
    } else {
        std::unique_ptr<Quadtree> nw, ne, se, sw;
#ifdef PARALLEL
        if (depth <= 1) { // Don't overload the system.
            // Spawn 3 threads, and assign 1 subquadrant to each of them.
            auto nwf = std::async(std::launch::async, bottom_up_impl, quadrant->nw(), detail_threshold, height - 1, depth + 1);
            auto nef = std::async(std::launch::async, bottom_up_impl, quadrant->ne(), detail_threshold, height - 1, depth + 1);
            auto sef = std::async(std::launch::async, bottom_up_impl, quadrant->se(), detail_threshold, height - 1, depth + 1);
            nw = nwf.get();
            ne = nef.get();
            se = sef.get();
            // Assign the remaining subquadrant to the current thread.
            sw = bottom_up_impl(quadrant->sw(), detail_threshold, height - 1, depth + 1);
        } else {
            nw = bottom_up_impl(quadrant->nw(), detail_threshold, height - 1, depth + 1);
            ne = bottom_up_impl(quadrant->ne(), detail_threshold, height - 1, depth + 1);
            se = bottom_up_impl(quadrant->se(), detail_threshold, height - 1, depth + 1);
            sw = bottom_up_impl(quadrant->sw(), detail_threshold, height - 1, depth + 1);
        }
#else
        nw = bottom_up_impl(quadrant->nw(), detail_threshold, height - 1, depth + 1);
        ne = bottom_up_impl(quadrant->ne(), detail_threshold, height - 1, depth + 1);
        se = bottom_up_impl(quadrant->se(), detail_threshold, height - 1, depth + 1);
        sw = bottom_up_impl(quadrant->sw(), detail_threshold, height - 1, depth + 1);
#endif
        auto mean = combine_means(*nw, *ne, *se, *sw);
        auto std = combine_stds(*nw, *ne, *se, *sw, mean);

        if (should_merge(detail_threshold, height, std)) {
#ifdef LOG_QUADTREE_BUILD
            std::cout << std::string(depth * 4, ' ') << "leaf\n";
#endif
            return std::make_unique<Quadtree>(height, depth,
                                              quadrant->i, quadrant->j,
                                              quadrant->n_rows, quadrant->n_cols,
                                              Quadtree::Leaf{},
                                              mean, std);
        } else {
#ifdef LOG_QUADTREE_BUILD
            std::cout << std::string(depth * 4, ' ') << "split\n";
#endif
            return std::make_unique<Quadtree>(height, depth,
                                              quadrant->i, quadrant->j,
                                              quadrant->n_rows, quadrant->n_cols,
                                              Quadtree::Fork{std::move(nw), std::move(ne), std::move(sw), std::move(se)},
                                              mean, std);
        }
    }
}

std::unique_ptr<Quadtree> bottom_up(std::unique_ptr<Quadrant> quadrant, double detail_threshold, int max_depth) {
    return bottom_up_impl(std::move(quadrant), detail_threshold, max_depth, 0);
}

#endif // BOTTOM_UP_H
