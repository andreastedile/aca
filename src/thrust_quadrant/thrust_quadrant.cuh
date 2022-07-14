#ifndef ACA_CUDA_QUADRANT_H
#define ACA_CUDA_QUADRANT_H

#include "../thrust_soa/thrust_soa.cuh" // ThrustDevicePixelSoa
#include "../quadtree/quadrant.h"   // Quadrant, RGB, Pixel

#include <memory> // unique_ptr

class ThrustQuadrant final : public Quadrant {
  public:
    ThrustQuadrant(int i, int j, int n_rows, int n_cols, const ThrustDevicePixelSoa& soa);
    ThrustQuadrant(int i, int j,
                  int n_rows, int n_cols,
                  const ThrustDevicePixelSoa& soa, int left, int length);

    [[nodiscard]] RGB<double> mean() const override;
    [[nodiscard]] RGB<double> sq_mean() const override;
    [[nodiscard]] std::unique_ptr<Quadrant> nw() const override;
    [[nodiscard]] std::unique_ptr<Quadrant> ne() const override;
    [[nodiscard]] std::unique_ptr<Quadrant> se() const override;
    [[nodiscard]] std::unique_ptr<Quadrant> sw() const override;

  private:
    const ThrustDevicePixelSoa& soa;
    const int left;
    const int length;
};

#endif // ACA_CUDA_QUADRANT_H
