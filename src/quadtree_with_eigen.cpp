#include <chrono>
#include <iostream>
#include <utility> // move
#include <variant>

#include <eigen3/Eigen/Eigen>

#include "quadtree_with_eigen.h"

RgbSoa::RgbSoa(unsigned n_pixels)
    : r{n_pixels}, g{n_pixels}, b{n_pixels} {}

Quadtree::Fork::Fork(std::unique_ptr<const Quadtree> nw, std::unique_ptr<const Quadtree> ne,
                     std::unique_ptr<const Quadtree> se, std::unique_ptr<const Quadtree> sw)
    : nw(std::move(nw)), ne(std::move(ne)), se(std::move(se)), sw(std::move(sw)) {
}

Quadtree::Leaf::Leaf(color_t r, color_t g, color_t b)
    : r(r), g(g), b(b) {}

Quadtree::Quadtree(unsigned int depth, unsigned int i, unsigned int j, unsigned int n_rows, unsigned int n_cols,
                   std::shared_ptr<QuadtreeLogger> logger)
    : depth(depth), i(i), j(j), n_rows(n_rows), n_cols(n_cols), data(Empty{}), m_logger(std::move(logger)) {
    m_logger->print_depth(depth);
    m_logger->print_i_j(i, j);
    m_logger->print_n_rows_n_cols(n_rows, n_cols);
}

Quadtree::Quadtree(unsigned int n_rows, unsigned int n_cols)
    : Quadtree(0, 0, 0, n_rows, n_cols, std::make_shared<QuadtreeMuteLogger>()) {
}

void Quadtree::build(const RgbSoa& image, unsigned left, unsigned right) {
    auto dim = right - left - 1;

    double r_mean = image.r.middleCols(left, dim).cast<double>().mean();
    double g_mean = image.g.middleCols(left, dim).cast<double>().mean();
    double b_mean = image.b.middleCols(left, dim).cast<double>().mean();
    m_logger->print_mean(r_mean, g_mean, b_mean);

    double r_sq_mean = image.r.middleCols(left, dim).cast<double>().square().mean();
    double g_sq_mean = image.g.middleCols(left, dim).cast<double>().square().mean();
    double b_sq_mean = image.b.middleCols(left, dim).cast<double>().square().mean();
    m_logger->print_sq_mean(r_sq_mean, g_sq_mean, b_sq_mean);

    double r_stddev = std::sqrt(r_sq_mean - std::pow(r_mean, 2));
    double g_stddev = std::sqrt(g_sq_mean - std::pow(g_mean, 2));
    double b_stddev = std::sqrt(b_sq_mean - std::pow(b_mean, 2));
    m_logger->print_stdev(r_stddev, g_stddev, b_stddev);

    if (depth < MAX_DEPTH &&
        r_stddev > DETAIL_THRESHOLD &&
        g_stddev > DETAIL_THRESHOLD &&
        b_stddev > DETAIL_THRESHOLD) {
        m_logger->print_split();

        // clang-format off
        auto nw = std::make_unique<Quadtree>(depth + 1, i +     0, j +     0, n_rows / 2, n_cols / 2, m_logger);
        auto ne = std::make_unique<Quadtree>(depth + 1, i +     0, j + n_cols / 2, n_rows / 2, n_cols / 2, m_logger);
        auto sw = std::make_unique<Quadtree>(depth + 1, i + n_rows / 2, j +     0, n_rows / 2, n_cols / 2, m_logger);
        auto se = std::make_unique<Quadtree>(depth + 1, i + n_rows / 2, j + n_cols / 2, n_rows / 2, n_cols / 2,m_logger);
        // clang-format on
        auto child_dim = (right - left) / 4;
        nw->build(image, left + 0 * child_dim, left + 1 * child_dim);
        ne->build(image, left + 1 * child_dim, left + 2 * child_dim);
        sw->build(image, left + 2 * child_dim, left + 3 * child_dim);
        se->build(image, left + 3 * child_dim, left + 4 * child_dim);

        data.emplace<Fork>(std::move(nw), std::move(ne), std::move(sw), std::move(se));
    } else {
        m_logger->print_leaf();
        data.emplace<Leaf>(r_mean, g_mean, b_mean);
    }
}

void Quadtree::set_logger(std::shared_ptr<QuadtreeLogger> logger) {
    m_logger = std::move(logger);
}
