#include "image.hpp"
#include "rgb.hpp"

// https://github.com/nothings/stb/blob/master/stb_image.h#L12
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// https://github.com/nothings/stb/blob/master/stb_image_write.h#L7
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <exception>
#include <iostream>

Image::Image(const std::string& filename) {
    int n;
    unsigned char* data = stbi_load(filename.c_str(), &w, &h, &n, 3);
    if (!data) {
        // Todo: throw execption instead
        std::cerr << "File " << filename << " does not exist\n";
        return;
    }

    this->data = new RGB[w * h];

    for (int i = 0; i < n_pixels(); i++) {
        this->data[i].R = data[i * 3 + 0];
        this->data[i].G = data[i * 3 + 1];
        this->data[i].B = data[i * 3 + 2];
    }

    stbi_image_free(data);

    row_offset = 0;
    col_offset = 0;

    W = w;
    H = h;

    std::cout << "width: " << w << ", height: " << h << "\n";
}

Image::Image(RGB* data, int W, int H, int row_offset, int col_offset, int w, int h)
    : data(data), W(W), H(H), row_offset(row_offset), col_offset(col_offset), w(w), h(h) {
}

int Image::n_pixels() {
    return w * h;
}

inline RGB& Image::operator()(unsigned row, unsigned col) {
    if (row >= h || col >= w)
        throw std::out_of_range("Matrix subscript out of bounds");
    return data[(row + row_offset) * W + (col_offset + col)];
}

inline RGB Image::operator()(unsigned row, unsigned col) const {
    if (row >= h || col >= w)
        throw std::out_of_range("const Matrix subscript out of bounds");
    return data[(row + row_offset) * W + (col_offset + col)];
}

void Image::write_to_file(const std::string& filename) {
    unsigned char* stb_data = new unsigned char[n_pixels() * 3];
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            stb_data[i * w * 3 + j * 3 + 0] = operator()(i, j).R;
            stb_data[i * w * 3 + j * 3 + 1] = operator()(i, j).G;
            stb_data[i * w * 3 + j * 3 + 2] = operator()(i, j).B;
        }
    }
    stbi_write_jpg(filename.c_str(), w, h, 3, stb_data, 100);
    stbi_image_free(stb_data);
}
