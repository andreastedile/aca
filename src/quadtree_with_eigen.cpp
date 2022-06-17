#include <chrono>
#include <iostream>
#include <utility> // move
#include <variant>

#include <eigen3/Eigen/Eigen>

#include "quadtree_with_eigen.h"

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

RgbSoa::RgbSoa(ColorVec r, ColorVec g, ColorVec b)
    : r(std::move(r)), g(std::move(g)), b(std::move(b)) {}

Quadtree::Fork::Fork(std::unique_ptr<const Quadtree> nw, std::unique_ptr<const Quadtree> ne,
                     std::unique_ptr<const Quadtree> se, std::unique_ptr<const Quadtree> sw)
    : nw(std::move(nw)), ne(std::move(ne)), se(std::move(se)), sw(std::move(sw)),
      n_leaves(nw->n_leaves() + ne->n_leaves() + sw->n_leaves() + se->n_leaves()) {
}

Quadtree::Leaf::Leaf(unsigned x, unsigned y, unsigned h, unsigned w, color_t r, color_t g, color_t b)
    : x(x), y(y), h(h), w(w), r(r), g(g), b(b) {}

Quadtree::Quadtree(unsigned int depth, unsigned int x, unsigned int y, unsigned int h, unsigned int w)
    : depth(depth), x(x), y(y), h(h), w(w), data(Empty{}) {
#ifndef NDEBUG
    std::cout << "Create node. Depth: " << depth << ", x: " << x << ", y: " << y << ", h: " << h << ", w: " << w << '\n';
#endif
}

Quadtree::Quadtree(unsigned int h, unsigned int w)
    : Quadtree(0, 0, 0, h, w) {}

void Quadtree::build_quadtree(const RgbSoa& image, unsigned left, unsigned right) {
    auto dim = right - left - 1;

    double r_mean = image.r.middleCols(left, dim).cast<double>().mean();
    double g_mean = image.g.middleCols(left, dim).cast<double>().mean();
    double b_mean = image.b.middleCols(left, dim).cast<double>().mean();

    double r_sq_mean = image.r.middleCols(left, dim).cast<double>().square().mean();
    double g_sq_mean = image.g.middleCols(left, dim).cast<double>().square().mean();
    double b_sq_mean = image.b.middleCols(left, dim).cast<double>().square().mean();

    double r_stddev = std::sqrt(r_sq_mean - std::pow(r_mean, 2));
    double g_stddev = std::sqrt(g_sq_mean - std::pow(g_mean, 2));
    double b_stddev = std::sqrt(b_sq_mean - std::pow(b_mean, 2));

    if (depth < MAX_DEPTH &&
        r_stddev > DETAIL_THRESHOLD &&
        g_stddev > DETAIL_THRESHOLD &&
        b_stddev > DETAIL_THRESHOLD) {

        // clang-format off
        auto nw = std::make_unique<Quadtree>(depth + 1, x +     0, y +     0, h / 2, w / 2);
        auto ne = std::make_unique<Quadtree>(depth + 1, x +     0, y + w / 2, h / 2, w / 2);
        auto sw = std::make_unique<Quadtree>(depth + 1, x + h / 2, y +     0, h / 2, w / 2);
        auto se = std::make_unique<Quadtree>(depth + 1, x + h / 2, y + w / 2, h / 2, w / 2);
        // clang-format on
        auto child_dim = (right - left) / 4;
        nw->build_quadtree(image, left + 0 * child_dim, left + 1 * child_dim);
        ne->build_quadtree(image, left + 1 * child_dim, left + 2 * child_dim);
        sw->build_quadtree(image, left + 2 * child_dim, left + 3 * child_dim);
        se->build_quadtree(image, left + 3 * child_dim, left + 4 * child_dim);

        data.emplace<Fork>(std::move(nw), std::move(ne), std::move(sw), std::move(se));
    } else {
        data.emplace<Leaf>(x, y, h, w, r_mean, g_mean, b_mean);
    }
}

std::vector<Quadtree::Leaf> Quadtree::get_leaves() const {
    std::vector<Leaf> leaves(n_leaves(), {0, 0, 0, 0, 0, 0, 0});
    get_leaves(leaves, 0);
    return leaves;
}

void Quadtree::get_leaves(std::vector<Leaf>& leaves, unsigned i) const {
    return std::visit(overloaded{
                          [](const Quadtree::Empty& e) {},
                          [&](const Quadtree::Leaf& l) { leaves[i] = l; },
                          [&](const Quadtree::Fork& f) {
                              f.nw->get_leaves(leaves, i);
                              i += f.nw->n_leaves();
                              f.ne->get_leaves(leaves, i);
                              i += f.ne->n_leaves();
                              f.se->get_leaves(leaves, i);
                              i += f.se->n_leaves();
                              f.sw->get_leaves(leaves, i);
                          },
                      },
                      data);
}

unsigned Quadtree::n_leaves() const {
    return std::visit(overloaded{
                          [](const Quadtree::Empty& e) { return (unsigned)0; },
                          [](const Quadtree::Leaf& l) { return (unsigned)1; },
                          [](const Quadtree::Fork& f) { return f.n_leaves; },
                      },
                      data);
}
