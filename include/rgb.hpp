#pragma once

struct Vec3 {
    double R, G, B;
    Vec3() = default;
    Vec3(double R, double G, double B);
    Vec3 operator+(const Vec3& vec3) const;
};

struct RGB {
    unsigned char R, G, B;
    RGB& operator=(const Vec3& vec3) {
        R = (unsigned char)vec3.R;
        G = (unsigned char)vec3.G;
        B = (unsigned char)vec3.B;
        return *this;
    }
};
