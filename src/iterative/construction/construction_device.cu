#include "combination.h"
#include "construction_device.cuh"
#include "qtmath.h"

__device__ void init_quadtree_array_leaves_device(RGBAoS aos, Node* quadtree_nodes, int tree_height, int n_leaves_per_thread) {
    int n_higher_nodes = (pow_base4(tree_height) - 1) / 3;
    int block_offset = blockIdx.x * blockDim.x;

    auto read_ptr = aos.aos + n_leaves_per_thread * (block_offset + threadIdx.x);

    Node* current_thread_leaves = quadtree_nodes +
                                  n_higher_nodes +
                                  n_leaves_per_thread * (block_offset + threadIdx.x);

#pragma unroll
    for (int i = 0; i < n_leaves_per_thread; i++) {
        current_thread_leaves[i] = Node(
            {float(read_ptr[i].r), float(read_ptr[i].g), float(read_ptr[i].b)}, // mean
            {0.0, 0.0, 0.0},                                                    // std
            Node::Type::LEAF);
    }
}

__global__ void construct_quadtree_array_device(Node* g_nodes, int tree_height, RGBAoS aos, float detail_threshold, int n_leaves_per_thread) {
    init_quadtree_array_leaves_device(aos, g_nodes, tree_height, n_leaves_per_thread);
    __syncthreads();

    // Number of pixels contained in the subquadrant represented by a node at the level immediately below.
    // Below this level are the leaves, each representing 1 pixel.
    int n_nodes_at_higher_levels = (pow_base4(tree_height - 1) - 1) / 3;
    int current_level_n_nodes = pow_base4(tree_height - 1);
    Node* lower_level_nodes = g_nodes + n_nodes_at_higher_levels + current_level_n_nodes;
    Node* current_level_nodes = g_nodes + n_nodes_at_higher_levels;

#pragma unroll
    for (int n_nodes_to_produce = div4(n_leaves_per_thread);
         n_nodes_to_produce >= 1;
         n_nodes_to_produce = div4(n_nodes_to_produce)) {
        int thread_nodes_idx = n_nodes_to_produce * (blockIdx.x * blockDim.x + threadIdx.x);

#pragma unroll
        for (int i = 0; i < n_nodes_to_produce; i++) {
            int node_idx = thread_nodes_idx + i;
            int children_idx = times4(node_idx);
            current_level_nodes[node_idx] = make_internal_node(
                lower_level_nodes[children_idx + 0],
                lower_level_nodes[children_idx + 1],
                lower_level_nodes[children_idx + 2],
                lower_level_nodes[children_idx + 3],
                detail_threshold);
        }
        current_level_n_nodes = div4(current_level_n_nodes);
        lower_level_nodes = current_level_nodes;
        current_level_nodes = current_level_nodes - current_level_n_nodes;
    }

    __syncthreads();

    // Above we handle the cases where each threads produces at least one node.

    // We enter the following for when there are more threads than nodes to produce.

    for (
        // At the current level, in the current block, each threads reduce four nodes at the level immediately below
        int n_working_threads_per_block = div4(blockDim.x),
            // The current level in the tree that we are constructing
        curr_level = tree_height - 1;
        // We need a thread to reduce four nodes.
        n_working_threads_per_block >= 1;
        // Equivalently, we can terminate the construction when the number of nodes at the current level amounts to the number of blocks.
        // curr_level >= tree_height - log4(blockDim.x),
        n_working_threads_per_block = div4(n_working_threads_per_block),
            curr_level--) {
        if (threadIdx.x < n_working_threads_per_block) {
            int node_idx = blockIdx.x * n_working_threads_per_block + threadIdx.x;
            int children_idx = times4(node_idx);
            current_level_nodes[node_idx] = make_internal_node(
                lower_level_nodes[children_idx + 0],
                lower_level_nodes[children_idx + 1],
                lower_level_nodes[children_idx + 2],
                lower_level_nodes[children_idx + 3],
                detail_threshold);
        }

        __syncthreads();

        current_level_n_nodes = div4(current_level_n_nodes);
        lower_level_nodes = current_level_nodes;
        current_level_nodes = current_level_nodes - current_level_n_nodes;
    }
}
