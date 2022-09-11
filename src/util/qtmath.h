#pragma once

#ifdef __CUDACC__
#define CUDA_HOSTDEV __host__ __device__
#else
#define CUDA_HOSTDEV
#endif

// compute 2*n
CUDA_HOSTDEV int times2(int n);

// compute 2^n
CUDA_HOSTDEV int pow_base2(int n);

// compute 4^n = 2^(2n)
CUDA_HOSTDEV int pow_base4(int n);

CUDA_HOSTDEV int div2(int n);

CUDA_HOSTDEV int div4(int n);

CUDA_HOSTDEV int times4(int n);

CUDA_HOSTDEV int square(int n);

CUDA_HOSTDEV int log4(int n);

CUDA_HOSTDEV bool is_pow2(int n);

CUDA_HOSTDEV bool is_pow4(int n);
