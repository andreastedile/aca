#pragma once

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

    Quadtree(int i, int j, int n_rows, int n_cols, std::variant<Fork, Leaf> data, RGB<float> mean, RGB<float> std);

    [[nodiscard]] const std::variant<Fork, Leaf>& data() const;
    [[nodiscard]] RGB<unsigned char> color() const;
    [[nodiscard]] RGB<float> mean() const;
    [[nodiscard]] RGB<float> std() const;

    const int i;
    const int j;
    const int n_rows;
    const int n_cols;

  private:
    std::variant<Fork, Leaf> m_data;
    RGB<unsigned char> m_color;
    RGB<float> m_std;
    RGB<float> m_mean;
};
