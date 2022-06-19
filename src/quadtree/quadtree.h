#ifndef ACA_QUADTREE_H
#define ACA_QUADTREE_H

#include <memory> // unique_ptr
#include <string>
#include <tuple>
#include <variant>

#include "rgb.h"

class Quadtree {
  public:
    struct Fork final {
        std::unique_ptr<const Quadtree> nw;
        std::unique_ptr<const Quadtree> ne;
        std::unique_ptr<const Quadtree> se;
        std::unique_ptr<const Quadtree> sw;
        Fork(std::unique_ptr<const Quadtree> nw,
             std::unique_ptr<const Quadtree> ne,
             std::unique_ptr<const Quadtree> se,
             std::unique_ptr<const Quadtree> sw);
    };
    struct Leaf final {};
    using Empty = std::monostate;

    Quadtree(int max_depth, int n_rows, int n_cols);
    Quadtree(int height, int depth, int i, int j, int n_rows, int n_cols);

    virtual void build(double detail_threshold) final;
    [[nodiscard]] const std::variant<Fork, Leaf, Empty>& data() const;
    [[nodiscard]] virtual Pixel color() const final;

    const int height;
    const int depth;
    const int i;
    const int j;
    const int n_rows;
    const int n_cols;

  protected:
    [[nodiscard]] virtual Pixel mean() const = 0;
    [[nodiscard]] virtual RGB<double> sq_mean() const = 0;
    [[nodiscard]] virtual bool should_split(double detail_threshold, const RGB<double>& stdev) const final;

    [[nodiscard]] virtual std::unique_ptr<Quadtree> nw() const = 0;
    [[nodiscard]] virtual std::unique_ptr<Quadtree> ne() const = 0;
    [[nodiscard]] virtual std::unique_ptr<Quadtree> se() const = 0;
    [[nodiscard]] virtual std::unique_ptr<Quadtree> sw() const = 0;

  private:
    std::variant<Fork, Leaf, Empty> m_data;
    Pixel m_mean;

#ifdef LOG_QUADTREE_BUILD
    int indent;
#endif
};

#endif // ACA_QUADTREE_H
