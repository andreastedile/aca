#ifndef ACA_TOP_DOWN_H
#define ACA_TOP_DOWN_H

#include "quadrant.h"
#include "quadtree.h"

#include <memory>

bool should_split(double detail_threshold, int height, const RGB<double>& std);

std::unique_ptr<Quadtree> top_down(std::unique_ptr<Quadrant> quadrant, double detail_threshold);

#endif // ACA_TOP_DOWN_H
