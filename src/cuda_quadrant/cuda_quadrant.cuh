#ifndef ACA_CUDA_QUADRANT_H
#define ACA_CUDA_QUADRANT_H

#include "../cuda_soa/cuda_soa.cuh" // CudaDevicePixelSoa
#include "../quadtree/quadrant.h"   // Quadrant, RGB, Pixel

#include <memory> // unique_ptr

class CudaQuadrant final : public Quadrant {
  public:
    CudaQuadrant(int i, int j, int n_rows, int n_cols, const CudaDevicePixelSoa& soa);
    CudaQuadrant(int i, int j,
                  int n_rows, int n_cols,
                  const CudaDevicePixelSoa& soa, int left, int length);

    [[nodiscard]] RGB<double> mean() const override;
    [[nodiscard]] RGB<double> sq_mean() const override;
    [[nodiscard]] std::unique_ptr<Quadrant> nw() const override;
    [[nodiscard]] std::unique_ptr<Quadrant> ne() const override;
    [[nodiscard]] std::unique_ptr<Quadrant> se() const override;
    [[nodiscard]] std::unique_ptr<Quadrant> sw() const override;

  private:
    const CudaDevicePixelSoa& soa;
    const int left;
    const int length;
};

#endif // ACA_CUDA_QUADRANT_H
