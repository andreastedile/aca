#ifndef ACA_COLORIZER_H
#define ACA_COLORIZER_H

#include "../quadtree/quadtree.h"

#include <cstdint>

void colorize(uint8_t* pixels, int n_pixels, const Quadtree& quadtree);

#endif // ACA_COLORIZER_H
