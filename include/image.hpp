#pragma once

#include "rgb.hpp"

#include <memory>
#include <string>

class Image {
  private:
    Image(std::shared_ptr<RGB[]> data, int W, int H, int row_offset, int col_offset, int w, int h);
    std::shared_ptr<RGB[]> m_data;
    // Width of the original image
    int m_W;
    // Height of the original image
    int m_H;
    // X coordinate of the top-left pixel of this image, with respect to the original image
    int m_row_offset;
    // Y coordinate of the top-left pixel of this image, with respect to the original image
    int m_col_offset;

  public:
    // Width of this image
    int m_w;
    // Width of this image
    int m_h;
    [[nodiscard]] int n_pixels() const;
    explicit Image(const std::string& file_name);
    ~Image();
    RGB& operator()(unsigned row, unsigned col);
    RGB operator()(unsigned row, unsigned col) const;
    void write_to_file(const std::string& filename) const;
    void fill(const Vec3& color);
    [[nodiscard]] Vec3 compute_sum() const;
    [[nodiscard]] Vec3 compute_sq_sum() const;
    [[nodiscard]] Vec3 compute_mean() const;
    [[nodiscard]] Vec3 compute_sq_mean() const;
    [[nodiscard]] Vec3 compute_stddev() const;
    [[nodiscard]] Image* nw() const;
    [[nodiscard]] Image* ne() const;
    [[nodiscard]] Image* se() const;
    [[nodiscard]] Image* sw() const;
};
