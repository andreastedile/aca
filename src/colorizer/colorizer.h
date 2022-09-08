#ifndef ACA_COLORIZER_H
#define ACA_COLORIZER_H

#include "quadtree.h"

void colorize(unsigned char* pixels, int n_rows, int n_cols, const Quadtree& quadtree);

#endif // ACA_COLORIZER_H
