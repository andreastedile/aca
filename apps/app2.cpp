#include <chrono>
#include <iostream>

#include "file_utils.h"
#include "flatten.h"
#include "quadtree_with_eigen.h"

int main() {
    int w, h, n;
    uint8_t* data = read_file("image.jpg", &w, &h, &n);
    const int n_pixels = w * h;
    ColorVec r(n_pixels);
    ColorVec g(n_pixels);
    ColorVec b(n_pixels);
    // fill red, green, blue color vectors
    int i = 0;
    flatten_data(data, r, g, b, 0, 0, w, h, i, w);

    RgbSoa image(std::move(r), std::move(g), std::move(b));

    std::cout << "Start quadtree build\n";

    auto start = std::chrono::steady_clock::now();

    Quadtree root(h, w);
    root.build_quadtree(image, 0, n_pixels);
    // TODO: set in r, g, b the new color

    auto end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time in milliseconds: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";

    return 0;
}
