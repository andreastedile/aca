#pragma once

#include "node.h"

void colorize(unsigned char* pixels, int n_rows, int n_cols, const Node* quadtree_array, int colorize_up_to_level);
