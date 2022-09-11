#include "combination.h"
#include "qtmath.h"

__device__ __host__ bool should_merge(float detail_threshold, const RGB<float>& std) {
    return std.r <= detail_threshold &&
           std.g <= detail_threshold &&
           std.b <= detail_threshold;
}

// source
// https://stats.stackexchange.com/questions/25848/how-to-sum-a-standard-deviation/442050#442050
__device__ __host__ RGB<float> combine_means(const Node& nw, const Node& ne, const Node& se, const Node& sw) {
    // int n_pixels = 4 * n_pixels_subquadrant;
    RGB<float> nw_mean = nw.mean,
               ne_mean = ne.mean,
               se_mean = se.mean,
               sw_mean = sw.mean;
    return {
        (nw_mean.r + ne_mean.r + se_mean.r + sw_mean.r) / 4,
        (nw_mean.g + ne_mean.g + se_mean.g + sw_mean.g) / 4,
        (nw_mean.b + ne_mean.b + se_mean.b + sw_mean.b) / 4,
    };
    // return {
    //     (nw_mean.r * n_pixels_subquadrant + ne_mean.r * n_pixels_subquadrant + se_mean.r * n_pixels_subquadrant + sw_mean.r * n_pixels_subquadrant) / n_pixels,
    //     (nw_mean.g * n_pixels_subquadrant + ne_mean.g * n_pixels_subquadrant + se_mean.g * n_pixels_subquadrant + sw_mean.g * n_pixels_subquadrant) / n_pixels,
    //     (nw_mean.b * n_pixels_subquadrant + ne_mean.b * n_pixels_subquadrant + se_mean.b * n_pixels_subquadrant + sw_mean.b * n_pixels_subquadrant) / n_pixels,
    // };
}

__device__ __host__ RGB<float> combine_stds(const Node& nw, const Node& ne, const Node& se, const Node& sw, const RGB<float>& mean) {
    // int n_pixels = 4 * n_pixels_subquadrant;
    RGB<float> nw_mean = nw.mean,
               ne_mean = ne.mean,
               se_mean = se.mean,
               sw_mean = sw.mean;
    RGB<float> nw_std = nw.std,
               ne_std = ne.std,
               se_std = se.std,
               sw_std = sw.std;

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

__device__ __host__ Node make_internal_node(Node& nw, Node& ne, Node& se, Node& sw, float detail_threshold) {
    auto mean = combine_means(nw, ne, se, sw);
    auto std = combine_stds(nw, ne, se, sw, mean);
    if (should_merge(detail_threshold, std)) {
        return {mean, std, Node::Type::LEAF};
    } else {
        return {mean, std, Node::Type::FORK};
    }
}
