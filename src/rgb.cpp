#include "rgb.hpp"

#include <cmath>

Vec3::Vec3(double R, double G, double B) : R(R), G(G), B(B){};

Vec3 Vec3::operator+(const Vec3& vec3) const {
    return {R + vec3.R, G + vec3.G, B + vec3.B};
}
