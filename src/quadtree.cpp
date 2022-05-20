#include "quadtree.hpp"
#include "image.hpp"
#include "rgb.hpp"

#ifdef PARALLEL
#include <future>
#ifndef MAX_PARALLELISM_DEPTH
#define MAX_PARALLELISM_DEPTH 1
#endif
#endif

#ifndef NDEBUG
#include <iostream>
unsigned Quadtree::n_quadtrees = 0;
#endif

Quadtree::Quadtree(const std::string& filename) : image(new Image(filename)), depth(0) {
#ifndef NDEBUG
    n_quadtrees = 1;
    id = n_quadtrees;
#endif
}

Quadtree::Quadtree(Image* image, int depth) : image(image), depth(depth) {
#ifndef NDEBUG
    n_quadtrees++;
    id = n_quadtrees;
#endif
}

Quadtree::~Quadtree() {
    delete image;
}

int Quadtree::width() {
    return image->w;
}

int Quadtree::height() {
    return image->h;
}

int Quadtree::n_pixels() {
    return width() * height();
}

void Quadtree::write_to_file(const std::string& filename) {
    image->write_to_file(filename);
}

void Quadtree::build() {
    if (should_split()) {
#ifndef NDEBUG
        std::cout << depth << "/" << n_quadtrees << ": quadtree should split\n";
#endif
        nw = new Quadtree(image->nw(), depth + 1);
        ne = new Quadtree(image->ne(), depth + 1);
        se = new Quadtree(image->se(), depth + 1);
        sw = new Quadtree(image->sw(), depth + 1);

#ifdef PARALLEL
        // Recursion: split into 4 subquadrants.

        if (depth <= MAX_PARALLELISM_DEPTH) { // Don't overload the system.
            // Spawn 3 threads, and assign 1 subquadrant to each of them.
            std::future<void> ne_h = std::async(std::launch::async, &Quadtree::build, ne);
            std::future<void> se_h = std::async(std::launch::async, &Quadtree::build, se);
            std::future<void> sw_h = std::async(std::launch::async, &Quadtree::build, sw);
            // Assign the remaining subquadrant to the current thread.
            nw->build();
            delete nw;

            ne_h.get();
            delete ne;
            se_h.get();
            delete se;
            sw_h.get();
            delete sw;
        } else {
            nw->build();
            delete nw;
            ne->build();
            delete ne;
            se->build();
            delete se;
            sw->build();
            delete sw;
        }
#else
        nw->build();
        delete nw;
        ne->build();
        delete ne;
        se->build();
        delete se;
        sw->build();
        delete sw;
#endif
    } else {
#ifndef NDEBUG
        std::cout << depth << "/" << id << ": quadtree should fill\n";
#endif
        Vec3 mean = image->compute_mean();
        image->fill(mean);
    }
}

bool Quadtree::should_split() {
    if (depth == MAX_DEPTH)
        return false;
    Vec3 stddev = image->compute_stddev();
    return stddev.R > DETAIL_THRESHOLD && stddev.G > DETAIL_THRESHOLD && stddev.B > DETAIL_THRESHOLD;
}
