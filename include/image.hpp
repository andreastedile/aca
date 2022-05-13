#pragma once

#include "rgb.hpp"

#include <memory>
#include <string>

class Image {
  private:
    Image(std::shared_ptr<RGB[]> data, int W, int H, int row_offset, int col_offset, int w, int h);
    std::shared_ptr<RGB[]> data;
    // Width of the original image
    int W;
    // Height of the original image
    int H;
    // X coordinate of the top-left pixel of this image, with respect to the original image
    int row_offset;
    // Y coordinate of the top-left pixel of this image, with respect to the original image
    int col_offset;

  public:
    // Width of this image
    int w;
    // Width of this image
    int h;
    int n_pixels() const;
    Image(const std::string& file_name);
    ~Image();
    RGB& operator()(unsigned row, unsigned col);
    RGB operator()(unsigned row, unsigned col) const;
    void write_to_file(const std::string& filename) const;
    void fill(Vec3 color);
    Vec3 compute_sum() const;
    Vec3 compute_sq_sum() const;
    Vec3 compute_mean() const;
    Vec3 compute_sq_mean() const;
    Vec3 compute_stddev() const;
    Image* nw();
    Image* ne();
    Image* se();
    Image* sw();
};
