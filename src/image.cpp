#include "image.hpp"
#include "rgb.hpp"

// https://github.com/nothings/stb/blob/master/stb_image.h#L12
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// https://github.com/nothings/stb/blob/master/stb_image_write.h#L7
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdexcept>
#include <utility>
#ifndef NDEBUG
#include <iostream>
#endif

// Clang-Tidy: Constructor does not initialize these fields: m_w, m_h
Image::Image(const std::string& filename) { // NOLINT(cppcoreguidelines-pro-type-member-init)
    int n;
    unsigned char* data = stbi_load(filename.c_str(), &m_w, &m_h, &n, 3); // we initialize m_w, m_h here
    if (!data) {
        throw std::runtime_error("Could not open file " + filename);
        return;
    }

    m_data = std::shared_ptr<RGB[]>(new RGB[m_w * m_h]);

    for (int i = 0; i < n_pixels(); i++) {
        m_data[i].R = data[i * 3 + 0];
        m_data[i].G = data[i * 3 + 1];
        m_data[i].B = data[i * 3 + 2];
    }

    stbi_image_free(data);

    m_row_offset = 0;
    m_col_offset = 0;

    m_W = m_w;
    m_H = m_h;

#ifndef NDEBUG
    std::cout << "width: " << m_w << ", height: " << m_h << "\n";
#endif
}

Image::Image(std::shared_ptr<RGB[]> data, int W, int H, int row_offset, int col_offset, int w, int h)
    : m_data(std::move(data)), m_W(W), m_H(H), m_row_offset(row_offset), m_col_offset(col_offset), m_w(w), m_h(h) {
}

Image::~Image() { // NOLINT(modernize-use-equals-default)
#ifndef NDEBUG
    std::cout << m_data.use_count() - 1 << "\n";
#endif
}

int Image::n_pixels() const {
    return m_w * m_h;
}

inline RGB& Image::operator()(unsigned row, unsigned col) {
    if (row >= m_h || col >= m_w)
        throw std::out_of_range("Matrix subscript out of bounds");
    return m_data[(row + m_row_offset) * m_W + (m_col_offset + col)];
}

inline RGB Image::operator()(unsigned row, unsigned col) const {
    if (row >= m_h || col >= m_w)
        throw std::out_of_range("const Matrix subscript out of bounds");
    return m_data[(row + m_row_offset) * m_W + (m_col_offset + col)];
}

void Image::write_to_file(const std::string& filename) const {
    auto* stb_data = new unsigned char[n_pixels() * 3];
    for (int i = 0; i < m_h; i++) {
        for (int j = 0; j < m_w; j++) {
            stb_data[i * m_w * 3 + j * 3 + 0] = operator()(i, j).R;
            stb_data[i * m_w * 3 + j * 3 + 1] = operator()(i, j).G;
            stb_data[i * m_w * 3 + j * 3 + 2] = operator()(i, j).B;
        }
    }
    stbi_write_jpg(filename.c_str(), m_w, m_h, 3, stb_data, 100);
    stbi_image_free(stb_data);
}

Vec3 Image::compute_sum() const {
    Vec3 sum(0, 0, 0);
    for (int i = 0; i < m_h; i++) {
        for (int j = 0; j < m_w; j++) {
            sum.R += operator()(i, j).R;
            sum.G += operator()(i, j).G;
            sum.B += operator()(i, j).B;
        }
    }
    return sum;
}

Vec3 Image::compute_sq_sum() const {
    Vec3 sq_sum(0, 0, 0);
    for (int i = 0; i < m_h; i++) {
        for (int j = 0; j < m_w; j++) {
            sq_sum.R += std::pow(operator()(i, j).R, 2);
            sq_sum.G += std::pow(operator()(i, j).G, 2);
            sq_sum.B += std::pow(operator()(i, j).B, 2);
        }
    }
    return sq_sum;
}

Vec3 Image::compute_mean(const Vec3& sum) const {
    return {sum.R / n_pixels(), sum.G / n_pixels(), sum.B / n_pixels()};
}

Vec3 Image::compute_sq_mean(const Vec3& sq_sum) const {
    return {sq_sum.R / n_pixels(), sq_sum.G / n_pixels(), sq_sum.B / n_pixels()};
}

Vec3 Image::compute_stddev(const Vec3& mean, const Vec3& sq_mean) {
    return {std::sqrt(sq_mean.R - pow(mean.R, 2)), std::sqrt(sq_mean.G - pow(mean.G, 2)), std::sqrt(sq_mean.B - pow(mean.B, 2))};
}

void Image::fill(const Vec3& color) {
    for (int i = 0; i < m_h; i++) {
        for (int j = 0; j < m_w; j++) {
            operator()(i, j) = color;
        }
    }
}

Image* Image::nw() const {
    return new Image(m_data, m_W, m_H, m_row_offset, m_col_offset, m_w / 2, m_h / 2);
}
Image* Image::ne() const {
    return new Image(m_data, m_W, m_H, m_row_offset, m_col_offset + m_w / 2, m_w / 2, m_h / 2);
}
Image* Image::se() const {
    return new Image(m_data, m_W, m_H, m_row_offset + m_h / 2, m_col_offset + m_w / 2, m_w / 2, m_h / 2);
}
Image* Image::sw() const {
    return new Image(m_data, m_W, m_H, m_row_offset + m_h / 2, m_col_offset, m_w / 2, m_h / 2);
}
