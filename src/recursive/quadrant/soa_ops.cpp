#include "soa_ops.h"

#include <cassert>

RGB<float> compute_mean(const RGBSoA& soa, const Extents& extents) {
    return {soa.r.middleCols(left, length).cast<float>().compute_mean(),
            soa.g.middleCols(left, length).cast<float>().compute_mean(),
            soa.b.middleCols(left, length).cast<float>().compute_mean()};
}

RGB<float> compute_sq_mean(const RGBSoA& soa, const Extents& extents) {
    return {soa.r.middleCols(left, length).cast<float>().square().compute_mean(),
            soa.g.middleCols(left, length).cast<float>().square().compute_mean(),
            soa.b.middleCols(left, length).cast<float>().square().compute_mean()};
}

QuadrantSoA::QuadrantSoA(int i, int j, int n_rows, int n_cols, const RGBSoA& soa)
    : QuadrantSoA(i, j, n_rows, n_cols, soa, 0, n_rows * n_cols) {}

QuadrantSoA::QuadrantSoA(int i, int j, int n_rows, int n_cols, const RGBSoA& soa, int left, int length)
    : Extents(i, j, n_rows, n_cols), soa(soa), left(left), length(length) {
    assert(i >= 0);
    assert(j >= 0);
    assert(n_rows >= 1);
    assert(n_cols >= 1);
}

RGB<float> QuadrantSoA::mean() const {
}

RGB<float> QuadrantSoA::sq_mean() const {
}

std::unique_ptr<Extents> QuadrantSoA::nw() const {
    return std::make_unique<QuadrantSoA>(i + 0, j + 0,
                                         n_rows / 2, n_cols / 2,
                                         soa, left + 0 * (length / 4), length / 4);
}

std::unique_ptr<Extents> QuadrantSoA::ne() const {
    return std::make_unique<QuadrantSoA>(i + 0, j + n_cols / 2,
                                         n_rows / 2, n_cols / 2,
                                         soa, left + 1 * (length / 4), length / 4);
}

std::unique_ptr<Extents> QuadrantSoA::se() const {
    return std::make_unique<QuadrantSoA>(i + n_rows / 2, j + n_cols / 2,
                                         n_rows / 2, n_cols / 2,
                                         soa, left + 2 * (length / 4), length / 4);
}

std::unique_ptr<Extents> QuadrantSoA::sw() const {
    return std::make_unique<QuadrantSoA>(i + n_rows / 2, j + 0,
                                         n_rows / 2, n_cols / 2,
                                         soa, left + 3 * (length / 4), length / 4);
}
