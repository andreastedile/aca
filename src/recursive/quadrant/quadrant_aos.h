#pragma once

#include "quadrant.h"
#include "rgbaos.h"

class QuadrantAoS final : public Quadrant {
  public:
    QuadrantAoS(int i, int j, int n_rows, int n_cols, const RGBAoS& aos);
    QuadrantAoS(int i, int j, int n_rows, int n_cols, const RGBAoS& aos, int left, int length);

    [[nodiscard]] RGB<float> mean() const override;
    [[nodiscard]] RGB<float> sq_mean() const override;

    [[nodiscard]] std::unique_ptr<Quadrant> nw() const override;
    [[nodiscard]] std::unique_ptr<Quadrant> ne() const override;
    [[nodiscard]] std::unique_ptr<Quadrant> se() const override;
    [[nodiscard]] std::unique_ptr<Quadrant> sw() const override;

  private:
    const int left;
    const int length;
    const RGBAoS& aos;
};
