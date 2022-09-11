#pragma once

#ifdef __CUDACC__
#define CUDA_HOSTDEV __host__ __device__
#else
#define CUDA_HOSTDEV
#endif

template <typename T>
struct RGB {
    CUDA_HOSTDEV RGB(T r, T g, T b) : r(r), g(g), b(b) {}
    CUDA_HOSTDEV RGB() : RGB({}, {}, {}) {}
    T r;
    T g;
    T b;
};
