#pragma once

#include "quadrant.h"
#include "quadtree.h"

#include <memory>

bool should_merge(float detail_threshold, int height, const RGB<float>& std);

// source
// https://stats.stackexchange.com/questions/25848/how-to-sum-a-standard-deviation/442050#442050
RGB<float> combine_means(const Quadtree& nw, const Quadtree& ne, const Quadtree& se, const Quadtree& sw);

RGB<float> combine_stds(const Quadtree& nw, const Quadtree& ne, const Quadtree& se, const Quadtree& sw, const RGB<float>& mean);

std::unique_ptr<Quadtree> bottom_up(std::unique_ptr<Quadrant> quadrant, float detail_threshold);
