#pragma once

#include "quadrant.h"
#include "rgbsoa.h"

RGB<float> compute_mean(const RGBSoA& soa, const Extents& extents);

RGB<float> compute_sq_mean(const RGBSoA& soa, const Extents& extents);
