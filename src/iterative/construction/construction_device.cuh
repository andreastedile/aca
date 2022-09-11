#pragma once

#include "node.h"
#include "rgbaos.h"

__global__ void construct_quadtree_array_device(Node* g_nodes, int tree_height, RGBAoS aos, float detail_threshold, int n_leaves_per_thread);