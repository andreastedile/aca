#pragma once

#include "quadrant.h"
#include "rgb.h"
#include "rgbsoa.h"

#include <memory>

class Quadrant final {
  public:
    Quadrant(int i, int j, int n_rows, int n_cols, const RGBSoA& soa);
    Quadrant(int i, int j, int n_rows, int n_cols, const RGBSoA& soa, int left, int length);

    [[nodiscard]] RGB<float> mean() const;
    [[nodiscard]] RGB<float> sq_mean() const;

    [[nodiscard]] std::unique_ptr<Quadrant> nw() const;
    [[nodiscard]] std::unique_ptr<Quadrant> ne() const;
    [[nodiscard]] std::unique_ptr<Quadrant> se() const;
    [[nodiscard]] std::unique_ptr<Quadrant> sw() const;

    const int i;
    const int j;
    const int n_rows;
    const int n_cols;

  private:
    const RGBSoA& soa;
    const int left;
    const int length;
};
