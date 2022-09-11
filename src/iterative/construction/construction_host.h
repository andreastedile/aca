#pragma once

#include "node.h"
#include "rgbaos.h"

void init_quadtree_array_leaves_host(Node* quadtree_array, int tree_height, const RGBAoS& aos);

void construct_quadtree_array_host(Node* quadtree_array, int from_level, float detail_threshold);
