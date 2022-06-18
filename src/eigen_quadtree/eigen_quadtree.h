#ifndef ACA_EIGEN_QUADTREE_H
#define ACA_EIGEN_QUADTREE_H

#include "../eigen_soa/eigen_soa.h" // EigenPixelSoa
#include "../quadtree/quadtree.h"   // Quadtree, RGB, Pixel

#include <memory> // unique_ptr

class EigenQuadtree final : public Quadtree {
  public:
    EigenQuadtree(const EigenPixelSoa& soa, int n_rows, int n_cols, int max_depth);
    EigenQuadtree(int height, int depth,
                  int i, int j,
                  int n_rows, int n_cols,
                  const EigenPixelSoa& soa, int left, int length);

  protected:
    [[nodiscard]] Pixel mean() const override;
    [[nodiscard]] RGB<double> sq_mean() const override;
    [[nodiscard]] std::unique_ptr<Quadtree> nw() const override;
    [[nodiscard]] std::unique_ptr<Quadtree> ne() const override;
    [[nodiscard]] std::unique_ptr<Quadtree> se() const override;
    [[nodiscard]] std::unique_ptr<Quadtree> sw() const override;

  private:
    const EigenPixelSoa& soa;
    const int left;
    const int length;
};

#endif // ACA_EIGEN_QUADTREE_H
