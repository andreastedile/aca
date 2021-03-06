#ifndef ACA_RGB_H
#define ACA_RGB_H

#include <cstdint>

using color_t = uint8_t;

template <typename T>
struct RGB {
    T r;
    T g;
    T b;
};

using Pixel = RGB<color_t>;

#endif // ACA_RGB_H
