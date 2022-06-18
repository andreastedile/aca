#ifndef ACA_QUADTREE_WITH_EIGEN_H
#define ACA_QUADTREE_WITH_EIGEN_H

constexpr unsigned MAX_DEPTH = 8;
constexpr unsigned DETAIL_THRESHOLD = 13;

#include <cstdint> // uint8_t
#include <eigen3/Eigen/Eigen>
#include <memory> // unique_ptr
#include <variant>

using color_t = uint8_t;

// Array<typename Scalar, int RowsAtCompileTime, int ColsAtCompileTime>
using ColorVec = Eigen::Array<color_t, 1, Eigen::Dynamic>;

struct RgbSoa {
    ColorVec r, g, b;
    RgbSoa(unsigned n_pixels);
};

class Quadtree {
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
    struct Leaf {
        color_t r, g, b;
        Leaf(color_t r, color_t g, color_t b);
    };

    // Depth of this node in the quadtree
    unsigned depth;
    unsigned i, j;
    unsigned n_rows, n_cols;

    using Empty = std::monostate;
    std::variant<Fork, Leaf, Empty> data;

    Quadtree(unsigned depth, unsigned i, unsigned j, unsigned n_rows, unsigned n_cols);

    Quadtree(unsigned n_rows, unsigned n_cols);

    void build(const RgbSoa& image, unsigned left, unsigned right);
};

#endif // ACA_QUADTREE_WITH_EIGEN_H
