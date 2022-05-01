#include "image.hpp"

#include <iostream>

// https://github.com/nothings/stb/blob/master/stb_image.h#L12
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::Image(const std::string& filename) {
    this->filename = filename;

    data = stbi_load(filename.c_str(), &x, &y, &n, 3);
    if (!data) {
        // Todo: throw execption instead
        std::cerr << "File " << filename << " does not exist\n";
        return;
    }
    std::cout << "x: " << x << ", y: " << y << ", n: " << n << "\n";
}

Image::~Image() {
    stbi_image_free(data);
    std::cout << "Free called\n";
}
