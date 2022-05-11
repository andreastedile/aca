#include "quadtree.hpp"
#include "image.hpp"

#include <iostream>

Quadtree::Quadtree(const std::string& filename) : image(Image(filename)) {}

int Quadtree::width() {
    return image.w;
}

int Quadtree::height() {
    return image.h;
}

int Quadtree::n_pixels() {
    return width() * height();
}

void Quadtree::write_to_file(const std::string& filename) {
    image.write_to_file(filename);
}
