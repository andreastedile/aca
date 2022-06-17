#ifndef ACA_COLORIZER_H
#define ACA_COLORIZER_H

#include "quadtree_with_eigen.h"
#include <cstdlib>

void colorize(uint8_t* data, const Quadtree& quadtree);

#endif // ACA_COLORIZER_H
