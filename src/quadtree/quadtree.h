#ifndef ACA_QUADTREE_H
#define ACA_QUADTREE_H

#include <functional>
#include <memory> // unique_ptr
#include <string>
#include <tuple>
#include <variant>

#include "rgb.h"

class Quadtree final {
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

    Quadtree(int height, int depth, int i, int j, int n_rows, int n_cols, std::variant<Fork, Leaf> data, RGB<double> mean, RGB<double> std);

    [[nodiscard]] const std::variant<Fork, Leaf>& data() const;
    [[nodiscard]] virtual RGB<unsigned char> color() const final;
    [[nodiscard]] virtual RGB<double> mean() const final;
    [[nodiscard]] virtual RGB<double> std() const final;

    const int height;
    const int depth;
    const int i;
    const int j;
    const int n_rows;
    const int n_cols;

  private:
    std::variant<Fork, Leaf> m_data;
    RGB<unsigned char> m_color;
    RGB<double> m_std;
    RGB<double> m_mean;

#ifdef LOG_QUADTREE_BUILD
    int indent;
#endif
};

#endif // ACA_QUADTREE_H
