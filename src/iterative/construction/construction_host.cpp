#include "construction_host.h"
#include "combination.h"
#include "qtmath.h"
#include "rgb.h"

#include <cassert>
#include <cmath>
#include <spdlog/spdlog.h>

void init_quadtree_array_leaves_host(Node* quadtree_array, int tree_height, const RGBAoS& aos) {
    // Number of nodes that are above in the tree
    const int n_above = (pow_base4(tree_height) - 1) / 3;

    Node* quadtree_leaves = quadtree_array + n_above;

#ifndef NPARALLEL
#pragma omp parallel for
#endif
    for (int i = 0; i < aos.n_elements; i++) {
        quadtree_leaves[i] = Node(
            {float(aos.aos[i].r), float(aos.aos[i].g), float(aos.aos[i].b)}, // mean
            {0.0, 0.0, 0.0},                                                 // std
            Node::Type::LEAF);
    }
}

void construct_quadtree_array_host(Node* quadtree_array, int from_level, float detail_threshold) {
#ifdef DEBUG_CONSTRUCT_QUADTREE_HOST
    spdlog::debug("from level: {}", from_level);
#endif

#ifndef NPARALLEL
#pragma omp parallel
#endif
    for (int curr_level = from_level; curr_level >= 0; curr_level--) {
#ifdef DEBUG_CONSTRUCT_QUADTREE_HOST
        spdlog::debug("current level: {}", curr_level);
#endif

        // Number of nodes that are above in the tree
        const int n_above = (pow_base4(curr_level) - 1) / 3;

        // Number of nodes at the current level
        const int n_nodes = pow_base4(curr_level);

#ifdef DEBUG_CONSTRUCT_QUADTREE_HOST
        spdlog::debug("n above: {}, n nodes: {}", n_above, n_nodes);
#endif

        Node* reduce = quadtree_array + n_above + n_nodes;

        Node* dest = quadtree_array + n_above;

#ifndef NPARALLEL
#pragma omp for
#endif
        for (int i = 0; i < n_nodes; i++) {
            dest[i] = make_internal_node(
                reduce[times4(i) + 0],
                reduce[times4(i) + 1],
                reduce[times4(i) + 2],
                reduce[times4(i) + 3],
                detail_threshold);
        }
    }
}
