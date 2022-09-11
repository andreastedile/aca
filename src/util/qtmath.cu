#include "qtmath.h"

#include <cmath>

// compute 2*n
CUDA_HOSTDEV int times2(int n) {
    return n << 1;
}

// compute 2^n
CUDA_HOSTDEV int pow_base2(int n) {
    return 1 << n;
}

// compute 4^n = 2^(2n)
CUDA_HOSTDEV int pow_base4(int n) {
    return pow_base2(times2(n));
}

CUDA_HOSTDEV int div2(int n) {
    return n >> 1;
}

CUDA_HOSTDEV int div4(int n) {
    return n >> 2;
}

CUDA_HOSTDEV int times4(int n) {
    return n << 2;
}

CUDA_HOSTDEV int square(int n) {
    return n * n;
}

CUDA_HOSTDEV int log4(int n) {
    return div2(int(std::log2(n)));
}

CUDA_HOSTDEV bool is_pow2(int n) {
    return n == std::pow(2, int(std::log2(n)));
}

CUDA_HOSTDEV bool is_pow4(int n) {
    return n == std::pow(4, log4(n));
}
