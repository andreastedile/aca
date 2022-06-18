#include "quadtree.h"

#include <cassert>
#include <cmath>
#include <utility> // move

Quadtree::Fork::Fork(std::unique_ptr<const Quadtree> nw, std::unique_ptr<const Quadtree> ne,
                     std::unique_ptr<const Quadtree> se, std::unique_ptr<const Quadtree> sw)
    : nw(std::move(nw)), ne(std::move(ne)), se(std::move(se)), sw(std::move(sw)) {
}

Quadtree::Quadtree(int max_depth, int n_rows, int n_cols)
    : Quadtree(max_depth, 0, 0, 0, n_rows, n_cols) {}

Quadtree::Quadtree(int height, int depth, int i, int j, int n_rows, int n_cols)
    : height(height), depth(depth),
      i(i), j(j),
      n_rows(n_rows), n_cols(n_cols),
      m_data(Empty{}),
      m_mean()
#ifdef LOG_QUADTREE_BUILD
      ,
      m_logger(depth * 4)
#endif
{
    assert(height >= 0);
    assert(depth >= 0);
    assert(i >= 0);
    assert(j >= 0);
    assert(n_rows >= 1);
    assert(n_cols >= 1);

#ifdef LOG_QUADTREE_BUILD
    m_logger.print_depth(depth);
    m_logger.print_i_j(i, j);
    m_logger.print_n_rows_n_cols(n_rows, n_cols);
#endif
}

void Quadtree::build(double detail_threshold) {
    assert(detail_threshold >= 0);

    m_mean = this->mean();
#ifdef LOG_QUADTREE_BUILD
    m_logger.print_mean(m_mean);
#endif

    const auto sq_mean = this->sq_mean();
#ifdef LOG_QUADTREE_BUILD
    m_logger.print_sq_mean(sq_mean);
#endif

    const auto stdev = RGB<double>{
        std::sqrt(sq_mean.r - std::pow(m_mean.r, 2)),
        std::sqrt(sq_mean.g - std::pow(m_mean.g, 2)),
        std::sqrt(sq_mean.b - std::pow(m_mean.b, 2))};
#ifdef LOG_QUADTREE_BUILD
    m_logger.print_stdev(stdev);
#endif

    if (should_split(detail_threshold, stdev)) {
        m_logger.print_split();

        auto nw = this->nw();
        auto ne = this->ne();
        auto se = this->se();
        auto sw = this->sw();

        nw->build(detail_threshold);
        ne->build(detail_threshold);
        sw->build(detail_threshold);
        se->build(detail_threshold);

        m_data.emplace<Fork>(std::move(nw), std::move(ne), std::move(sw), std::move(se));
    } else {
        m_logger.print_leaf();
        m_data.emplace<Leaf>();
    }
}

bool Quadtree::should_split(double detail_threshold, const RGB<double>& stdev) const {
    return height > 0 &&
           stdev.r > detail_threshold &&
           stdev.g > detail_threshold &&
           stdev.b > detail_threshold;
}

const std::variant<Quadtree::Fork, Quadtree::Leaf, Quadtree::Empty>& Quadtree::data() const {
    return m_data;
}

Pixel Quadtree::color() const {
    return m_mean;
}
