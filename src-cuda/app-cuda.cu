#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept> // runtime_error
#include <string>
#include <utility>

#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/reduce.h>
#include <thrust/transform_reduce.h>

// https://github.com/nothings/stb/blob/master/stb_image.h#L12
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// https://github.com/nothings/stb/blob/master/stb_image_write.h#L7
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

struct Color {
    uint8_t r, g, b;
};

struct square {
    __device__ double operator()(const uint8_t& x) const {
        return double(x) * double(x);
    }
};

struct RgbSoa {
    thrust::device_vector<uint8_t> r, g, b;
    RgbSoa(const thrust::host_vector<uint8_t>& r, const thrust::host_vector<uint8_t>& g, const thrust::host_vector<uint8_t>& b) : r(r), g(g), b(b) {
    }
};

constexpr unsigned MAX_DEPTH = 8;
constexpr unsigned DETAIL_THRESHOLD = 13;

uint8_t* read_file(std::string filename, int* w, int* h, int* n) {
    uint8_t* data = stbi_load(filename.c_str(), w, h, n, 3);
    if (!data) {
        throw std::runtime_error("Could not open file 'images/funny.jpg'");
    }
    return data;
}

class Quadtree {
  public:
    std::unique_ptr<Quadtree> nw;
    std::unique_ptr<Quadtree> ne;
    std::unique_ptr<Quadtree> se;
    std::unique_ptr<Quadtree> sw;

    unsigned depth;
    unsigned x, y;
    unsigned h, w;

    Color color;

    // Constructs the root of a quadtree
    Quadtree(unsigned h, unsigned w)
        : depth(0), x(0), y(0), h(h), w(h) {
#ifndef NDEBUG
        std::cout << "Create quadtree root\n";
#endif
    }

    // Constructs an internal node of a quadtree. Todo: make private
    Quadtree(unsigned depth, unsigned x, unsigned y, unsigned h, unsigned w)
        : depth(depth), x(x), y(y), h(h), w(h) {
#ifndef NDEBUG
        std::cout << "Create quadtree node at depth " << depth << '\n';
#endif
    }

    void build_quadtree(const RgbSoa& image, unsigned left, unsigned right);

    unsigned n_pixels() const {
        return h * w;
    }
};

void Quadtree::build_quadtree(const RgbSoa& image, unsigned left, unsigned right) {
    auto should_split_start = std::chrono::steady_clock::now();

    auto r_sum = thrust::reduce(image.r.begin() + left, image.r.begin() + right, (double)0);
    auto g_sum = thrust::reduce(image.g.begin() + left, image.g.begin() + right, (double)0);
    auto b_sum = thrust::reduce(image.b.begin() + left, image.b.begin() + right, (double)0);

    auto r_sq_sum = thrust::transform_reduce(image.r.begin() + left, image.r.begin() + right, square(), (double)0, thrust::plus<double>());
    auto g_sq_sum = thrust::transform_reduce(image.g.begin() + left, image.g.begin() + right, square(), (double)0, thrust::plus<double>());
    auto b_sq_sum = thrust::transform_reduce(image.b.begin() + left, image.b.begin() + right, square(), (double)0, thrust::plus<double>());

    double r_mean = r_sum / image.r.size();
    double g_mean = g_sum / image.g.size();
    double b_mean = b_sum / image.b.size();

    double r_sq_mean = r_sq_sum / image.r.size();
    double g_sq_mean = g_sq_sum / image.g.size();
    double b_sq_mean = b_sq_sum / image.b.size();

    double r_stdev = std::sqrt(r_sq_mean - std::pow(r_mean, 2));
    double g_stdev = std::sqrt(g_sq_mean - std::pow(g_mean, 2));
    double b_stdev = std::sqrt(b_sq_mean - std::pow(b_mean, 2));

    auto should_split_end = std::chrono::steady_clock::now();
#ifndef NDEBUG
    std::cout << "Determine if split time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(should_split_end - should_split_start).count()
              << " ms\n";
#endif

    if (depth < MAX_DEPTH &&
        r_stdev > DETAIL_THRESHOLD &&
        g_stdev > DETAIL_THRESHOLD &&
        b_stdev > DETAIL_THRESHOLD) {

        nw = std::make_unique<Quadtree>(depth + 1, x, y, h / 2, w / 2);
        ne = std::make_unique<Quadtree>(depth + 1, x, y + w / 2, h / 2, w / 2);
        sw = std::make_unique<Quadtree>(depth + 1, x + h / 2, y, h / 2, w / 2);
        se = std::make_unique<Quadtree>(depth + 1, x + h / 2, y + w / 2, h / 2, w / 2);

        unsigned dim = (right - left) / 4;
        nw->build_quadtree(image, left + 0 * dim, left + 1 * dim);
        ne->build_quadtree(image, left + 1 * dim, left + 2 * dim);
        sw->build_quadtree(image, left + 2 * dim, left + 3 * dim);
        se->build_quadtree(image, left + 3 * dim, left + 4 * dim);
    } else {
        color = {(uint8_t)r_mean, (uint8_t)g_mean, (uint8_t)b_mean};
    }
}

void flatten_data(uint8_t* data,
                  thrust::host_vector<uint8_t>& r,
                  thrust::host_vector<uint8_t>& g,
                  thrust::host_vector<uint8_t>& b,
                  int top,
                  int left,
                  int w,
                  int h,
                  int& i,
                  int W) {
    if (w == 1) {
        r[i] = data[(top * W + left) * 3 + 0];
        g[i] = data[(top * W + left) * 3 + 1];
        b[i] = data[(top * W + left) * 3 + 2];
        i++;
    } else {
        flatten_data(data, r, g, b, top, left, w / 2, h / 2, i, W);
        flatten_data(data, r, g, b, top, left + w / 2, w / 2, h / 2, i, W);
        flatten_data(data, r, g, b, top + h / 2, left, w / 2, h / 2, i, W);
        flatten_data(data, r, g, b, top + h / 2, left + w / 2, w / 2, h / 2, i, W);
    }
}

void unflatten_data(uint8_t* data,
                    thrust::host_vector<uint8_t>& r,
                    thrust::host_vector<uint8_t>& g,
                    thrust::host_vector<uint8_t>& b,
                    int top,
                    int left,
                    int w,
                    int h,
                    int& i,
                    int W) {
    if (w == 1) {
        data[(top * W + left) * 3 + 0] = r[i];
        data[(top * W + left) * 3 + 1] = g[i];
        data[(top * W + left) * 3 + 2] = b[i];
        i++;
    } else {
        unflatten_data(data, r, g, b, top, left, w / 2, h / 2, i, W);
        unflatten_data(data, r, g, b, top, left + w / 2, w / 2, h / 2, i, W);
        unflatten_data(data, r, g, b, top + h / 2, left, w / 2, h / 2, i, W);
        unflatten_data(data, r, g, b, top + h / 2, left + w / 2, w / 2, h / 2, i, W);
    }
}

int main() {
    int w, h, n;
    uint8_t* data = read_file("images/funny.jpg", &w, &h, &n);
    int n_pixels = w * h;
    thrust::host_vector<uint8_t> r(n_pixels);
    thrust::host_vector<uint8_t> g(n_pixels);
    thrust::host_vector<uint8_t> b(n_pixels);
    // fill red, green, blue color vectors
    int i = 0;
    flatten_data(data, r, g, b, 0, 0, w, h, i, w);
    // for (int i = 0; i < n_pixels / 3; i++) {
    //     r[i] = data[i * 3 + 0];
    //     g[i] = data[i * 3 + 1];
    //     b[i] = data[i * 3 + 2];
    // }
    RgbSoa image(r, g, b);

    std::cout << "Start quadtree build\n";

    auto start = std::chrono::steady_clock::now();

    Quadtree root(h, w);
    root.build_quadtree(image, 0, n_pixels);
    // TODO: set in r, g, b the new color

    auto end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time in milliseconds: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";

    i = 0;
    unflatten_data(data, r, g, b, 0, 0, w, h, i, w);
    stbi_write_jpg("result.jpg", w, h, 3, data, 100);
    stbi_image_free(data);

    return 0;
}
