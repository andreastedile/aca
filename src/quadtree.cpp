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

Quadtree::Quadtree(const std::string &filename) : m_image(new Image(filename)), m_depth(0) {
#ifndef NDEBUG
    n_quadtrees = 1;
    id = n_quadtrees;
#endif
}

Quadtree::Quadtree(Image *image, int depth) : m_image(image), m_depth(depth) {
#ifndef NDEBUG
    n_quadtrees++;
    id = n_quadtrees;
#endif
}

Quadtree::~Quadtree() {
    delete m_image;
}

int Quadtree::width() const {
    return m_image->m_w;
}

int Quadtree::height() const {
    return m_image->m_h;
}

[[maybe_unused]] int Quadtree::n_pixels() const {
    return width() * height();
}

void Quadtree::write_to_file(const std::string &filename) const {
    m_image->write_to_file(filename);
}

void Quadtree::build() { // NOLINT(misc-no-recursion)
    if (should_split()) {
#ifndef NDEBUG
        std::cout << m_depth << "/" << n_quadtrees << ": quadtree should split\n";
#endif
        m_nw = new Quadtree(m_image->nw(), m_depth + 1);
        m_ne = new Quadtree(m_image->ne(), m_depth + 1);
        m_se = new Quadtree(m_image->se(), m_depth + 1);
        m_sw = new Quadtree(m_image->sw(), m_depth + 1);

#ifdef PARALLEL
        // Recursion: split into 4 subquadrants.

        if (m_depth <= MAX_PARALLELISM_DEPTH) { // Don't overload the system.
            // Spawn 3 threads, and assign 1 subquadrant to each of them.
            std::future<void> ne_h = std::async(std::launch::async, &Quadtree::build, m_ne);
            std::future<void> se_h = std::async(std::launch::async, &Quadtree::build, m_se);
            std::future<void> sw_h = std::async(std::launch::async, &Quadtree::build, m_sw);
            // Assign the remaining subquadrant to the current thread.
            m_nw->build();
            delete m_nw;

            ne_h.get();
            delete m_ne;
            se_h.get();
            delete m_se;
            sw_h.get();
            delete m_sw;
        } else {
            m_nw->build();
            delete m_nw;
            m_ne->build();
            delete m_ne;
            m_se->build();
            delete m_se;
            m_sw->build();
            delete m_sw;
        }
#else
        m_nw->build();
        delete m_nw;
        m_ne->build();
        delete m_ne;
        m_se->build();
        delete m_se;
        m_sw->build();
        delete m_sw;
#endif
    } else {
#ifndef NDEBUG
        std::cout << m_depth << "/" << id << ": quadtree should fill with: " << m_mean.R << ", " << m_mean.G << ", "
                  << m_mean.B << std::endl;
#endif
        m_image->fill(m_mean);
    }
}

bool Quadtree::should_split() {
    m_sum = m_image->compute_sum();
    m_sq_sum = m_image->compute_sq_sum();
    m_mean = m_image->compute_mean(m_sum);
    m_sq_mean = m_image->compute_sq_mean(m_sq_sum);
    m_stddev = Image::compute_stddev(m_mean, m_sq_mean);
    return m_depth < MAX_DEPTH && m_stddev.R > DETAIL_THRESHOLD && m_stddev.G > DETAIL_THRESHOLD &&
           m_stddev.B > DETAIL_THRESHOLD;
}
