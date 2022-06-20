#ifndef ACA_QUADRANT_H
#define ACA_QUADRANT_H

#include "rgb.h"

#include <cassert>
#include <memory>

class Quadrant {
  public:
    Quadrant(int i,
             int j,
             int n_rows,
             int n_cols) : i(i), j(j), n_rows(n_rows), n_cols(n_cols) {
        assert(i >= 0);
        assert(j >= 0);
        assert(n_rows >= 1);
        assert(n_cols >= 1);
    }
    [[nodiscard]] virtual Pixel mean() const = 0;
    [[nodiscard]] virtual RGB<double> sq_mean() const = 0;

    [[nodiscard]] virtual std::unique_ptr<Quadrant> nw() const = 0;
    [[nodiscard]] virtual std::unique_ptr<Quadrant> ne() const = 0;
    [[nodiscard]] virtual std::unique_ptr<Quadrant> se() const = 0;
    [[nodiscard]] virtual std::unique_ptr<Quadrant> sw() const = 0;

    int i;
    int j;
    int n_rows;
    int n_cols;
};

#endif // ACA_QUADRANT_H
