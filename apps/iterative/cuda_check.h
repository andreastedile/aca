#pragma once

#include <cstdio>
#include <cstdlib>

#define CHECK(call)                                                                                        \
    {                                                                                                      \
        auto error = call;                                                                                 \
        if (error != cudaSuccess) {                                                                        \
            fprintf(stderr, "Got error \"%s\" at %s:%d\n", cudaGetErrorString(error), __FILE__, __LINE__); \
            exit(EXIT_FAILURE);                                                                            \
        }                                                                                                  \
    }
