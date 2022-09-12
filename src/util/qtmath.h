#pragma once

#include <cmath>

// compute 2*n
#define times2(n) ((n) << 1)

// compute 2^n
#define pow_base2(n) (1 << (n))

// compute 4^n = 2^(2n)
#define pow_base4(n) pow_base2(times2(n))

#define div2(n) ((n) >> 1)

#define div4(n) ((n) >> 2)

#define times4(n) ((n) << 2)

#define square(n) ((n) * (n))

#define log4(n) div2(int(std::log2(n)))

#define is_pow2(n) ((n) == std::pow(2, int(std::log2(n))))

#define is_pow4(n) ((n) == std::pow(4, log4(n)))
