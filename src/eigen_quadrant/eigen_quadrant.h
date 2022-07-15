#ifndef ACA_EIGEN_QUADRANT_H
#define ACA_EIGEN_QUADRANT_H

#include "../eigen_soa/eigen_soa.h" // EigenPixelSoa
#include "../quadtree/quadrant.h"   // Quadrant, RGB, Pixel

class EigenQuadrant final : public Quadrant<EigenQuadrant> {
  public:
    EigenQuadrant(int i, int j, int n_rows, int n_cols, const EigenPixelSoa& soa);
    EigenQuadrant(int i, int j,
                  int n_rows, int n_cols,
                  const EigenPixelSoa& soa, int left, int length);

    [[nodiscard]] RGB<double> mean_impl() const;
    [[nodiscard]] RGB<double> sq_mean_impl() const;
    [[nodiscard]] EigenQuadrant nw_impl() const;
    [[nodiscard]] EigenQuadrant ne_impl() const;
    [[nodiscard]] EigenQuadrant se_impl() const;
    [[nodiscard]] EigenQuadrant sw_impl() const;

  private:
    const EigenPixelSoa& soa;
    const int left;
    const int length;
};

#endif // ACA_EIGEN_QUADRANT_H
