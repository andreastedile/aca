#pragma once

#include "rgb.h"

#ifdef __CUDACC__
#define CUDA_HOSTDEV __host__ __device__
#else
#define CUDA_HOSTDEV
#endif

class Node {
  public:
    // https://github.com/NVIDIA/libcudacxx/issues/162
    enum class Type {
        LEAF,
        FORK
    };

    CUDA_HOSTDEV Node(const RGB<float>& mean, const RGB<float>& std, Type type);

    // https://docs.nvidia.com/cuda/cuda-compiler-driver-nvcc/index.html#nvcc-identification-macro
    CUDA_HOSTDEV Node();

    RGB<float> mean;
    RGB<float> std;
    Type type;

    [[nodiscard]] RGB<unsigned char> color() const {
        return {(unsigned char)mean.r, (unsigned char)mean.g, (unsigned char)mean.b};
    }
};
