#include "quadtree.hpp"

// https://github.com/nothings/stb/blob/master/stb_image.h#L12
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// https://github.com/nothings/stb/blob/master/stb_image_write.h#L7
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

Quadtree::Quadtree(const std::string& filename) {
    int n;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &n, 3);
    if (!data) {
        // Todo: throw execption instead
        std::cerr << "File " << filename << " does not exist\n";
        return;
    }

    image = new RGB*[width];
    for (int i = 0; i < width; i++) {
        image[i] = new RGB[height];
    }

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            image[i][j].R = data[i * height * 3 + j * 3 + 0];
            image[i][j].G = data[i * height * 3 + j * 3 + 1];
            image[i][j].B = data[i * height * 3 + j * 3 + 2];
        }
    }

    stbi_image_free(data);

    x = 0;
    y = 0;

    std::cout << "width: " << width << ", height: " << height << "\n";
}

int Quadtree::n_pixels() {
    return width * height;
}

void Quadtree::write_to_file(const std::string& filename) {
    unsigned char* data = new unsigned char[n_pixels() * 3];
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            data[i * height * 3 + j * 3 + 0] = image[i][j].R;
            data[i * height * 3 + j * 3 + 1] = image[i][j].G;
            data[i * height * 3 + j * 3 + 2] = image[i][j].B;
        }
    }
    stbi_write_jpg(filename.c_str(), width, height, 3, data, 100);
    stbi_image_free(data);
}
