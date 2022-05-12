#include "quadtree.hpp"
#include "image.hpp"
#include "rgb.hpp"

#ifndef NDEBUG
#include <iostream>
unsigned Quadtree::n_quadtrees = 0;
#endif

Quadtree::Quadtree(const std::string& filename) : image(Image(filename)), depth(0) {
#ifndef NDEBUG
    n_quadtrees++;
#endif
}

Quadtree::Quadtree(Image image, int depth) : image(image), depth(depth) {
#ifndef NDEBUG
    n_quadtrees++;
#endif
}

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

void Quadtree::build() {
    if (should_split()) {
#ifndef NDEBUG
        std::cout << depth << "/" << n_quadtrees << ": quadtree should split\n";
#endif
        nw = new Quadtree(image.nw(), depth + 1);
        nw->build();
        ne = new Quadtree(image.ne(), depth + 1);
        ne->build();
        se = new Quadtree(image.se(), depth + 1);
        se->build();
        sw = new Quadtree(image.sw(), depth + 1);
        sw->build();
    } else {
#ifndef NDEBUG
        std::cout << depth << "/" << n_quadtrees << ": quadtree should fill\n";
#endif
        Vec3 mean = image.compute_mean();
        image.fill(mean);
    }
}

bool Quadtree::should_split() {
    if (depth == MAX_DEPTH)
        return false;
    Vec3 mean = image.compute_mean();
    return mean.R > DETAIL_THRESHOLD && mean.G > DETAIL_THRESHOLD && mean.B > DETAIL_THRESHOLD;
}
