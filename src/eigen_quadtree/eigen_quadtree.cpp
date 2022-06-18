#include "eigen_quadtree.h"

EigenQuadtree::EigenQuadtree(const EigenPixelSoa& soa, int n_rows, int n_cols, int max_depth)
    : EigenQuadtree(max_depth, 0, 0, 0, n_rows, n_cols, soa, 0, n_rows * n_cols) {}

EigenQuadtree::EigenQuadtree(int height, int depth,
                             int i, int j,
                             int n_rows, int n_cols,
                             const EigenPixelSoa& soa, int left, int length)
    : Quadtree(height, depth, i, j, n_rows, n_cols),
      soa(soa), left(left), length(length) {}

Pixel EigenQuadtree::mean() const {
    return {static_cast<color_t>(soa.r.middleCols(left, length).cast<double>().mean()),
            static_cast<color_t>(soa.g.middleCols(left, length).cast<double>().mean()),
            static_cast<color_t>(soa.b.middleCols(left, length).cast<double>().mean())};
}

RGB<double> EigenQuadtree::sq_mean() const {
    return {soa.r.middleCols(left, length).cast<double>().square().mean(),
            soa.g.middleCols(left, length).cast<double>().square().mean(),
            soa.b.middleCols(left, length).cast<double>().square().mean()};
}

std::unique_ptr<Quadtree> EigenQuadtree::nw() const {
    return std::make_unique<EigenQuadtree>(height - 1, depth + 1,
                                           i + 0, j + 0,
                                           n_rows / 2, n_cols / 2,
                                           soa, left + 0 * (length / 4), length / 4);
}

std::unique_ptr<Quadtree> EigenQuadtree::ne() const {
    return std::make_unique<EigenQuadtree>(height - 1, depth + 1,
                                           i + 0, j + n_cols / 2,
                                           n_rows / 2, n_cols / 2,
                                           soa, left + 1 * (length / 4), length / 4);
}
std::unique_ptr<Quadtree> EigenQuadtree::se() const {
    return std::make_unique<EigenQuadtree>(height - 1, depth + 1,
                                           i + n_rows / 2, j + 0,
                                           n_rows / 2, n_cols / 2,
                                           soa, left + 2 * (length / 4), length / 4);
}
std::unique_ptr<Quadtree> EigenQuadtree::sw() const {
    return std::make_unique<EigenQuadtree>(height - 1, depth + 1,
                                           i + n_rows / 2, j + n_cols / 2,
                                           n_rows / 2, n_cols / 2,
                                           soa, left + 3 * (length / 4), length / 4);
}
