#include "quadrant.h"

#include <cassert>

Quadrant::Quadrant(int i, int j, int n_rows, int n_cols, const RGBSoA& soa)
    : Quadrant(i, j, n_rows, n_cols, soa, 0, n_rows * n_cols) {}

Quadrant::Quadrant(int i, int j, int n_rows, int n_cols, const RGBSoA& soa, int left, int length)
    : i(i), j(j), n_rows(n_rows), n_cols(n_cols), soa(soa), left(left), length(length) {
    assert(i >= 0);
    assert(j >= 0);
    assert(n_rows >= 1);
    assert(n_cols >= 1);
}

RGB<float> Quadrant::mean() const {
    return {soa.r.middleCols(left, length).cast<float>().mean(),
            soa.g.middleCols(left, length).cast<float>().mean(),
            soa.b.middleCols(left, length).cast<float>().mean()};
}

RGB<float> Quadrant::sq_mean() const {
    return {soa.r.middleCols(left, length).cast<float>().square().mean(),
            soa.g.middleCols(left, length).cast<float>().square().mean(),
            soa.b.middleCols(left, length).cast<float>().square().mean()};
}

std::unique_ptr<Quadrant> Quadrant::nw() const {
    return std::make_unique<Quadrant>(i + 0, j + 0,
                                      n_rows / 2, n_cols / 2,
                                      soa, left + 0 * (length / 4), length / 4);
}

std::unique_ptr<Quadrant> Quadrant::ne() const {
    return std::make_unique<Quadrant>(i + 0, j + n_cols / 2,
                                      n_rows / 2, n_cols / 2,
                                      soa, left + 1 * (length / 4), length / 4);
}

std::unique_ptr<Quadrant> Quadrant::se() const {
    return std::make_unique<Quadrant>(i + n_rows / 2, j + 0,
                                      n_rows / 2, n_cols / 2,
                                      soa, left + 2 * (length / 4), length / 4);
}

std::unique_ptr<Quadrant> Quadrant::sw() const {
    return std::make_unique<Quadrant>(i + n_rows / 2, j + n_cols / 2,
                                      n_rows / 2, n_cols / 2,
                                      soa, left + 3 * (length / 4), length / 4);
}
