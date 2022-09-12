#pragma once

#include "quadrant.h"
#include "quadtree.h"

#include <memory>

bool should_merge(float detail_threshold, int height, const RGB<float>& std);

// source
// https://stats.stackexchange.com/questions/25848/how-to-sum-a-standard-deviation/442050#442050
RGB<float> combine_means(const QtNode& nw, const QtNode& ne, const QtNode& se, const QtNode& sw);

RGB<float> combine_stds(const QtNode& nw, const QtNode& ne, const QtNode& se, const QtNode& sw, const RGB<float>& mean);

std::unique_ptr<QtNode> bottom_up(std::unique_ptr<Extents> quadrant, float detail_threshold);
