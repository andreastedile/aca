#pragma once

#include "image.hpp"

#include <string>

#define MAX_DEPTH 8
#define DETAIL_THRESHOLD 13

class Quadtree {
  private:
#ifndef NDEBUG
    static unsigned n_quadtrees;
    unsigned id;
#endif
    // Constructs a quadtree internal node.
    Quadtree(Image* image, int depth);
    Image* const m_image;
    Quadtree* m_nw = nullptr;
    Quadtree* m_ne = nullptr;
    Quadtree* m_se = nullptr;
    Quadtree* m_sw = nullptr;

  public:
    // Constructs the root of the quadtree, loading the m_image specified by the filename.
    explicit Quadtree(const std::string& filename);
    ~Quadtree();
    // Depth of this subquadrant in the quadtree
    const int m_depth;
    [[nodiscard]] inline int width() const;
    [[nodiscard]] inline int height() const;
    // Returns the number of pixels of the m_image represented by this subquadrant.
    [[maybe_unused]] [[nodiscard]] inline int n_pixels() const;
    // Writes the quadtree's content to an m_image specified by the filename.
    void write_to_file(const std::string& filename) const;
    void build();
    [[nodiscard]] bool should_split() const;
};
