#pragma once

#include "quadrant.h"
#include "rgbsoa.h"

class QuadrantSoA final : public Quadrant {
  public:
    QuadrantSoA(int i, int j, int n_rows, int n_cols, const RGBSoA& soa);
    QuadrantSoA(int i, int j, int n_rows, int n_cols, const RGBSoA& soa, int left, int length);

    RGB<float> mean() const override;
    RGB<float> sq_mean() const override;

    [[nodiscard]] std::unique_ptr<Quadrant> nw() const override;
    [[nodiscard]] std::unique_ptr<Quadrant> ne() const override;
    [[nodiscard]] std::unique_ptr<Quadrant> se() const override;
    [[nodiscard]] std::unique_ptr<Quadrant> sw() const override;

  private:
    const int left;
    const int length;
    const RGBSoA& soa;
};
