#pragma once

#include "image.hpp"

#include <string>

#define MAX_DEPTH 8
#define DETAIL_THRESHOLD 13

class OldQuadtree {
  private:
#ifndef NDEBUG
    static unsigned n_quadtrees;
    unsigned id;
#endif
    // Constructs a quadtree internal node.
    OldQuadtree(Image* image, int depth);
    Image* const m_image;
    OldQuadtree* m_nw = nullptr;
    OldQuadtree* m_ne = nullptr;
    OldQuadtree* m_se = nullptr;
    OldQuadtree* m_sw = nullptr;
    Vec3 m_sum{};
    Vec3 m_sq_sum{};
    Vec3 m_mean{};
    Vec3 m_sq_mean{};
    Vec3 m_stddev{};

  public:
    // Constructs the root of the quadtree, loading the m_image specified by the filename.
    explicit OldQuadtree(const std::string& filename);
    ~OldQuadtree();
    // Depth of this subquadrant in the quadtree
    const int m_depth;
    [[nodiscard]] inline int width() const;
    [[nodiscard]] inline int height() const;
    // Returns the number of pixels of the m_image represented by this subquadrant.
    [[maybe_unused]] [[nodiscard]] inline int n_pixels() const;
    // Writes the quadtree's content to an m_image specified by the filename.
    void write_to_file(const std::string& filename) const;
    void build();
    [[nodiscard]] bool should_split();
};
