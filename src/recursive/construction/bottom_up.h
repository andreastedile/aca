#ifndef ACA_BOTTOM_UP_H
#define ACA_BOTTOM_UP_H

#include "quadrant.h"
#include "quadtree.h"

#include <memory>

bool should_merge(double detail_threshold, int height, const RGB<double>& std);

// source
// https://stats.stackexchange.com/questions/25848/how-to-sum-a-standard-deviation/442050#442050
RGB<double> combine_means(const Quadtree& nw, const Quadtree& ne, const Quadtree& se, const Quadtree& sw);

RGB<double> combine_stds(const Quadtree& nw, const Quadtree& ne, const Quadtree& se, const Quadtree& sw, const RGB<double>& mean);

std::unique_ptr<Quadtree> bottom_up(std::unique_ptr<Quadrant> quadrant, double detail_threshold);

#endif // ACA_BOTTOM_UP_H
