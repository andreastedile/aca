#ifndef ACA_EIGEN_QUADRANT_H
#define ACA_EIGEN_QUADRANT_H

#include "../eigen_soa/eigen_soa.h" // EigenPixelSoa
#include "../quadtree/quadrant.h"   // Quadrant, RGB, Pixel

#include <memory> // unique_ptr

class EigenQuadrant final : public Quadrant {
  public:
    EigenQuadrant(int i, int j, int n_rows, int n_cols, const EigenPixelSoa& soa);
    EigenQuadrant(int i, int j,
                  int n_rows, int n_cols,
                  const EigenPixelSoa& soa, int left, int length);

    [[nodiscard]] Pixel mean() const override;
    [[nodiscard]] RGB<double> sq_mean() const override;
    [[nodiscard]] std::unique_ptr<Quadrant> nw() const override;
    [[nodiscard]] std::unique_ptr<Quadrant> ne() const override;
    [[nodiscard]] std::unique_ptr<Quadrant> se() const override;
    [[nodiscard]] std::unique_ptr<Quadrant> sw() const override;

  private:
    const EigenPixelSoa& soa;
    const int left;
    const int length;
};

#endif // ACA_EIGEN_QUADRANT_H
