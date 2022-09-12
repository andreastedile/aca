#pragma once

#include "quadrant.h"
#include "quadtree.h"

#include <memory>

bool should_split(float detail_threshold, int height, const RGB<float>& std);

std::unique_ptr<QtNode> top_down(std::unique_ptr<Extents> quadrant, float detail_threshold);
