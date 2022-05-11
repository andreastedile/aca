#pragma once

#include "image.hpp"

#include <string>

class Quadtree {
  private:
    Image image;

  public:
    // Constructs the root of the quadtree, loading the image specified by the filename.
    Quadtree(const std::string& filename);
    int width();
    int height();
    // Returns the number of pixels of the image represented by this subquadrant.
    int n_pixels();
    // Writes the quadtree's content to an image specified by the filename.
    void write_to_file(const std::string& filename);
};
