#include <chrono>
#include <iostream>
#include <utility> // move

#include <eigen3/Eigen/Eigen>

#include "quadtree_with_eigen.h"

RgbSoa::RgbSoa(ColorVec r, ColorVec g, ColorVec b)
    : r(std::move(r)), g(std::move(g)), b(std::move(b)) {}

Quadtree::Fork::Fork(std::unique_ptr<const Quadtree> nw, std::unique_ptr<const Quadtree> ne, std::unique_ptr<const Quadtree> se, std::unique_ptr<const Quadtree> sw)
    : nw(std::move(nw)), ne(std::move(ne)), se(std::move(se)), sw(std::move(sw)) {}

Quadtree::Leaf::Leaf(color_t r, color_t g, color_t b)
    : r(r), g(g), b(b) {}

Quadtree::Quadtree(unsigned int depth, unsigned int x, unsigned int y, unsigned int h, unsigned int w)
    : depth(depth), x(x), y(y), h(h), w(w), data(Empty{}) {
#ifndef NDEBUG
    std::cout << "Create node. Depth: " << depth << ", x: " << x << ", y: " << y << ", h: " << h << ", w: " << w << '\n';
#endif
}

Quadtree::Quadtree(unsigned int h, unsigned int w)
    : Quadtree(0, 0, 0, h, w) {}

void Quadtree::build_quadtree(const RgbSoa& image, unsigned left, unsigned right) {
    auto dim = (right - left) / 4;

    double r_sum = image.r.middleCols(left, dim).cast<double>().sum();
    double g_sum = image.g.middleCols(left, dim).cast<double>().sum();
    double b_sum = image.b.middleCols(left, dim).cast<double>().sum();

    double r_sq_sum = image.r.middleCols(left, dim).cast<double>().square().sum();
    double g_sq_sum = image.g.middleCols(left, dim).cast<double>().square().sum();
    double b_sq_sum = image.b.middleCols(left, dim).cast<double>().square().sum();

    auto r_mean = (uint8_t)(r_sum / (double)image.r.size());
    auto g_mean = (uint8_t)(g_sum / (double)image.g.size());
    auto b_mean = (uint8_t)(b_sum / (double)image.b.size());

    double r_sq_mean = r_sq_sum / (double)image.r.size();
    double g_sq_mean = g_sq_sum / (double)image.g.size();
    double b_sq_mean = b_sq_sum / (double)image.b.size();

    double r_stdev = std::sqrt(r_sq_mean - std::pow(r_mean, 2));
    double g_stdev = std::sqrt(g_sq_mean - std::pow(g_mean, 2));
    double b_stdev = std::sqrt(b_sq_mean - std::pow(b_mean, 2));

    if (depth < MAX_DEPTH &&
        r_stdev > DETAIL_THRESHOLD &&
        g_stdev > DETAIL_THRESHOLD &&
        b_stdev > DETAIL_THRESHOLD) {

        std::cout << "Should split\n";

        // clang-format off
        auto nw = std::make_unique<Quadtree>(depth + 1, x + 0,     y + 0,     h / 2, w / 2);
        auto ne = std::make_unique<Quadtree>(depth + 1, x + 0,     y + w / 2, h / 2, w / 2);
        auto sw = std::make_unique<Quadtree>(depth + 1, x + h / 2, y + 0,     h / 2, w / 2);
        auto se = std::make_unique<Quadtree>(depth + 1, x + h / 2, y + w / 2, h / 2, w / 2);
        // clang-format on

        nw->build_quadtree(image, left + 0 * dim, left + 1 * dim);
        ne->build_quadtree(image, left + 1 * dim, left + 2 * dim);
        sw->build_quadtree(image, left + 2 * dim, left + 3 * dim);
        se->build_quadtree(image, left + 3 * dim, left + 4 * dim);

        data.emplace<Fork>(std::move(nw), std::move(ne), std::move(sw), std::move(se));
    } else {
        std::cout << "Leaf\n";
        data.emplace<Leaf>(r_mean, g_mean, b_mean);
    }
}
