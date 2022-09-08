#include "quadtree.h"

#include <cassert>
#include <spdlog/spdlog.h> // LOG_QUADTREE
#include <utility>

Quadtree::Fork::Fork(std::unique_ptr<const Quadtree> nw, std::unique_ptr<const Quadtree> ne,
                     std::unique_ptr<const Quadtree> se, std::unique_ptr<const Quadtree> sw)
    : nw(std::move(nw)), ne(std::move(ne)), se(std::move(se)), sw(std::move(sw)) {
}

Quadtree::Quadtree(int i, int j, int n_rows, int n_cols, std::variant<Fork, Leaf> data, RGB<double> mean, RGB<double> std)
    : i(i), j(j),
      n_rows(n_rows), n_cols(n_cols),
      m_data(std::move(data)),
      m_std(std),
      m_mean(mean),
      m_color{static_cast<unsigned char>(mean.r), static_cast<unsigned char>(mean.g), static_cast<unsigned char>(mean.b)} {
    assert(i >= 0);
    assert(j >= 0);
    assert(n_rows >= 1);
    assert(n_cols >= 1);

#ifdef LOG_QUADTREE
    spdlog::debug("height: {}, depth: {}, i: {}, j: {}, n rows: {}, n cols: {}", height, depth, i, j, n_rows, n_cols);
#endif
}

const std::variant<Quadtree::Fork, Quadtree::Leaf>& Quadtree::data() const {
    return m_data;
}

RGB<unsigned char> Quadtree::color() const {
    return m_color;
}

RGB<double> Quadtree::mean() const {
    return m_mean;
}

RGB<double> Quadtree::std() const {
    return m_std;
}
