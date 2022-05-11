#pragma once

#include <string>

struct RGB {
    unsigned char R, G, B;
};

class Quadtree {
  private:
    RGB** image;
    int width;
    int height;
    int x;
    int y;

  public:
    // Constructs the root of the quadtree, loading the image specified by the filename.
    Quadtree(const std::string& filename);
    // Returns the number of pixels of the image represented by this subquadrant.
    int n_pixels();
    // Writes the quadtree's content to an image specified by the filename.
    void write_to_file(const std::string& filename);
};
