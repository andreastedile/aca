#ifndef ACA_CUDA_QUADRANT_H
#define ACA_CUDA_QUADRANT_H

#include "../quadtree/quadrant.h"       // Quadrant, RGB, Pixel
#include "../thrust_soa/thrust_soa.cuh" // ThrustDevicePixelSoa

class ThrustQuadrant final : public Quadrant<ThrustQuadrant> {
  public:
    ThrustQuadrant(int i, int j, int n_rows, int n_cols, const ThrustDevicePixelSoa& soa);
    ThrustQuadrant(int i, int j,
                   int n_rows, int n_cols,
                   const ThrustDevicePixelSoa& soa, int left, int length);

    [[nodiscard]] RGB<double> mean_impl() const;
    [[nodiscard]] RGB<double> sq_mean_impl() const;
    [[nodiscard]] ThrustQuadrant nw_impl() const;
    [[nodiscard]] ThrustQuadrant ne_impl() const;
    [[nodiscard]] ThrustQuadrant se_impl() const;
    [[nodiscard]] ThrustQuadrant sw_impl() const;

  private:
    const ThrustDevicePixelSoa& soa;
    const int left;
    const int length;
};

#endif // ACA_CUDA_QUADRANT_H
