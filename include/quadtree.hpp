#pragma once

#include "image.hpp"

#include <string>

#define MAX_DEPTH 8
#define DETAIL_THRESHOLD 13

class Quadtree {
  private:
#ifndef NDEBUG
    static unsigned n_quadtrees;
#endif
    Image image;
    Quadtree *nw, *ne, *se, *sw;

  public:
    // Constructs the root of the quadtree, loading the image specified by the filename.
    Quadtree(const std::string& filename);
    // Constructs a quadtree internal node.
    Quadtree(Image image, int depth);
    // Depth of this subquadrant in the quadtree
    const int depth;
    int width();
    int height();
    // Returns the number of pixels of the image represented by this subquadrant.
    int n_pixels();
    // Writes the quadtree's content to an image specified by the filename.
    void write_to_file(const std::string& filename);
    void build();
    bool should_split();
};
