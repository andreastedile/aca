#ifndef ACA_QUADTREE_H
#define ACA_QUADTREE_H

#include "rgb.h"

#include <memory>
#include <variant>

class Quadtree final {
  public:
    struct Fork {
        std::unique_ptr<const Quadtree> nw;
        std::unique_ptr<const Quadtree> ne;
        std::unique_ptr<const Quadtree> se;
        std::unique_ptr<const Quadtree> sw;
        Fork(std::unique_ptr<const Quadtree> nw,
             std::unique_ptr<const Quadtree> ne,
             std::unique_ptr<const Quadtree> se,
             std::unique_ptr<const Quadtree> sw);
    };
    struct Leaf {};

    Quadtree(int i, int j, int n_rows, int n_cols, std::variant<Fork, Leaf> data, RGB<double> mean, RGB<double> std);

    [[nodiscard]] const std::variant<Fork, Leaf>& data() const;
    [[nodiscard]] RGB<unsigned char> color() const;
    [[nodiscard]] RGB<double> mean() const;
    [[nodiscard]] RGB<double> std() const;

    const int i;
    const int j;
    const int n_rows;
    const int n_cols;

  private:
    std::variant<Fork, Leaf> m_data;
    RGB<unsigned char> m_color;
    RGB<double> m_std;
    RGB<double> m_mean;
};

#endif // ACA_QUADTREE_H
