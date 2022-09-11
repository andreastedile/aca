#pragma once

#include "node.h"
#include "rgb.h"

#ifdef __CUDACC__
#define CUDA_HOSTDEV __host__ __device__
#else
#define CUDA_HOSTDEV
#endif

CUDA_HOSTDEV bool should_merge(float detail_threshold, const RGB<float>& std);

CUDA_HOSTDEV RGB<float> combine_means(const Node& nw, const Node& ne, const Node& se, const Node& sw);

CUDA_HOSTDEV RGB<float> combine_stds(const Node& nw, const Node& ne, const Node& se, const Node& sw, const RGB<float>& mean);

CUDA_HOSTDEV Node make_internal_node(Node& nw, Node& ne, Node& se, Node& sw, float detail_threshold);
