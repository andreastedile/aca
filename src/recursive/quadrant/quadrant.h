#pragma once

#include "quadrant.h"
#include "rgb.h"

#include <memory>

class Quadrant {
  public:
    Quadrant(int i, int j, int n_rows, int n_cols);

    [[nodiscard]] virtual std::unique_ptr<Quadrant> nw() const = 0;
    [[nodiscard]] virtual std::unique_ptr<Quadrant> ne() const = 0;
    [[nodiscard]] virtual std::unique_ptr<Quadrant> se() const = 0;
    [[nodiscard]] virtual std::unique_ptr<Quadrant> sw() const = 0;

    [[nodiscard]] virtual RGB<float> mean() const = 0;
    [[nodiscard]] virtual RGB<float> sq_mean() const = 0;

    const int i;
    const int j;
    const int n_rows;
    const int n_cols;
};
