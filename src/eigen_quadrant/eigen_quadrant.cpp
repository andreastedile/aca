#include "eigen_quadrant.h"

EigenQuadrant::EigenQuadrant(int i, int j, int n_rows, int n_cols, const EigenPixelSoa& soa) : EigenQuadrant(i, j, n_rows, n_cols, soa, 0, n_rows * n_cols) {}

EigenQuadrant::EigenQuadrant(int i, int j, int n_rows, int n_cols, const EigenPixelSoa& soa, int left, int length)
    : Quadrant(i, j, n_rows, n_cols), soa(soa), left(left), length(length) {}

Pixel EigenQuadrant::mean() const {
    return {static_cast<color_t>(soa.r.middleCols(left, length).cast<double>().mean()),
            static_cast<color_t>(soa.g.middleCols(left, length).cast<double>().mean()),
            static_cast<color_t>(soa.b.middleCols(left, length).cast<double>().mean())};
}

RGB<double> EigenQuadrant::sq_mean() const {
    return {soa.r.middleCols(left, length).cast<double>().square().mean(),
            soa.g.middleCols(left, length).cast<double>().square().mean(),
            soa.b.middleCols(left, length).cast<double>().square().mean()};
}

std::unique_ptr<Quadrant> EigenQuadrant::nw() const {
    return std::make_unique<EigenQuadrant>(i + 0, j + 0,
                                           n_rows / 2, n_cols / 2,
                                           soa, left + 0 * (length / 4), length / 4);
}

std::unique_ptr<Quadrant> EigenQuadrant::ne() const {
    return std::make_unique<EigenQuadrant>(i + 0, j + n_cols / 2,
                                           n_rows / 2, n_cols / 2,
                                           soa, left + 1 * (length / 4), length / 4);
}

std::unique_ptr<Quadrant> EigenQuadrant::se() const {
    return std::make_unique<EigenQuadrant>(i + n_rows / 2, j + 0,
                                           n_rows / 2, n_cols / 2,
                                           soa, left + 2 * (length / 4), length / 4);
}

std::unique_ptr<Quadrant> EigenQuadrant::sw() const {
    return std::make_unique<EigenQuadrant>(i + n_rows / 2, j + n_cols / 2,
                                           n_rows / 2, n_cols / 2,
                                           soa, left + 3 * (length / 4), length / 4);
}
