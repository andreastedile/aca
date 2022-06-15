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
    RgbSoa(ColorVec r, ColorVec g, ColorVec b);
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
    unsigned x, y;
    unsigned h, w;

    using Empty = std::monostate;
    std::variant<Fork, Leaf, Empty> data;

    Quadtree(unsigned depth, unsigned x, unsigned y, unsigned h, unsigned w);

    Quadtree(unsigned h, unsigned w);

    void build_quadtree(const RgbSoa& image, unsigned left, unsigned right);
};

#endif // ACA_QUADTREE_WITH_EIGEN_H
