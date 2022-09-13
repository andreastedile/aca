#pragma once

#include "qtmath.h"

#include <cstdio>
#include <cstdlib>
#include <spdlog/spdlog.h>

#define CHECK(call)                                                                                        \
    {                                                                                                      \
        auto error = call;                                                                                 \
        if (error != cudaSuccess) {                                                                        \
            fprintf(stderr, "Got error \"%s\" at %s:%d\n", cudaGetErrorString(error), __FILE__, __LINE__); \
            exit(EXIT_FAILURE);                                                                            \
        }                                                                                                  \
    }

void check_nblocks_nthreads(int n_blocks, int n_threads, int n_pixels) {
    if (n_blocks * n_threads > n_pixels) {
        spdlog::error("Number of total threads {} exceeds the number of pixels {}", n_blocks * n_threads, n_pixels);
        std::exit(EXIT_FAILURE);
    }

    if (!is_pow4(n_blocks)) {
        spdlog::error("The number of blocks {} is not a power of four", n_blocks);
        std::exit(EXIT_FAILURE);
    }

    if (!is_pow4(n_threads)) {
        spdlog::error("The number of threads per block {} is not a power of four", n_blocks);
        std::exit(EXIT_FAILURE);
    }
}