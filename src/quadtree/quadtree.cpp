#include "quadtree.h"

#include <cassert>
#include <cmath>
#include <utility> // move
#ifdef LOG_QUADTREE_BUILD
#include <iostream>
#include <string>
#endif
Quadtree::Fork::Fork(std::unique_ptr<const Quadtree> nw, std::unique_ptr<const Quadtree> ne,
                     std::unique_ptr<const Quadtree> se, std::unique_ptr<const Quadtree> sw)
    : nw(std::move(nw)), ne(std::move(ne)), se(std::move(se)), sw(std::move(sw)) {
}

Quadtree::Quadtree(int height, int depth, int i, int j, int n_rows, int n_cols, std::variant<Fork, Leaf> data, RGB<double> mean, RGB<double> std)
    : height(height), depth(depth),
      i(i), j(j),
      n_rows(n_rows), n_cols(n_cols),
      m_data(std::move(data)),
      m_std(std),
      m_mean(mean),
      m_color{static_cast<color_t>(mean.r), static_cast<color_t>(mean.g), static_cast<color_t>(mean.b)}
#ifdef LOG_QUADTREE_BUILD
      ,
      indent(depth * 4)
#endif
{
    assert(height >= 0);
    assert(depth >= 0);
    assert(i >= 0);
    assert(j >= 0);
    assert(n_rows >= 1);
    assert(n_cols >= 1);

#ifdef LOG_QUADTREE_BUILD
    std::cout << std::string(indent, ' ') << "depth: " << depth << '\n';
    std::cout << std::string(indent, ' ') << "i: " << i << ", j: " << j << '\n';
    std::cout << std::string(indent, ' ') << "rows: " << n_rows << ", cols: " << n_cols << '\n';
#endif
}

const std::variant<Quadtree::Fork, Quadtree::Leaf>& Quadtree::data() const {
    return m_data;
}

Pixel Quadtree::color() const {
    return m_color;
}

RGB<double> Quadtree::mean() const {
    return m_mean;
}

RGB<double> Quadtree::std() const {
    return m_std;
}
