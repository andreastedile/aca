#include "quadrant_aos.h"

#include <cassert>

QuadrantAoS::QuadrantAoS(int i, int j, int n_rows, int n_cols, const RGBAoS& aos)
    : QuadrantAoS(i, j, n_rows, n_cols, aos, 0, n_rows * n_cols) {}

QuadrantAoS::QuadrantAoS(int i, int j, int n_rows, int n_cols, const RGBAoS& aos, int left, int length)
    : Quadrant(i, j, n_rows, n_cols), aos(aos), left(left), length(length) {
    assert(i >= 0);
    assert(j >= 0);
    assert(n_rows >= 1);
    assert(n_cols >= 1);
}

RGB<float> QuadrantAoS::mean() const {
    RGB<float> mean{0, 0, 0};
    for (int i = left; i < left + length; i++) {
        mean.r += aos.aos[i].r;
        mean.g += aos.aos[i].g;
        mean.b += aos.aos[i].b;
    }
    mean.r /= length;
    mean.g /= length;
    mean.b /= length;
    return mean;
}

RGB<float> QuadrantAoS::sq_mean() const {
    return {};
}

std::unique_ptr<Quadrant> QuadrantAoS::nw() const {
    return std::make_unique<QuadrantAoS>(i + 0, j + 0,
                                         n_rows / 2, n_cols / 2,
                                         aos, left + 0 * (length / 4), length / 4);
}

std::unique_ptr<Quadrant> QuadrantAoS::ne() const {
    return std::make_unique<QuadrantAoS>(i + 0, j + n_cols / 2,
                                         n_rows / 2, n_cols / 2,
                                         aos, left + 1 * (length / 4), length / 4);
}

std::unique_ptr<Quadrant> QuadrantAoS::se() const {
    return std::make_unique<QuadrantAoS>(i + n_rows / 2, j + n_cols / 2,
                                         n_rows / 2, n_cols / 2,
                                         aos, left + 2 * (length / 4), length / 4);
}

std::unique_ptr<Quadrant> QuadrantAoS::sw() const {
    return std::make_unique<QuadrantAoS>(i + n_rows / 2, j + 0,
                                         n_rows / 2, n_cols / 2,
                                         aos, left + 3 * (length / 4), length / 4);
}
