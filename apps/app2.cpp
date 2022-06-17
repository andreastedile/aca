#include <chrono>
#include <iostream>

#include "colorizer.h"
#include "file_utils.h"
#include "flatten.h"
#include "quadtree_with_eigen.h"

int main() {
    int w, h, n;
    uint8_t* data = read_file("image.jpg", &w, &h, &n);

    auto start = std::chrono::steady_clock::now();

    const int n_pixels = w * h;
    ColorVec r(n_pixels);
    ColorVec g(n_pixels);
    ColorVec b(n_pixels);
    // fill red, green, blue color vectors
    flatten_data(data, r, g, b, 0, 0, w, h, 0, w);

    RgbSoa image(std::move(r), std::move(g), std::move(b));

    std::cout << "Start quadtree build\n";

    Quadtree root(h, w);
    root.build_quadtree(image, 0, n_pixels);

    colorize(data, root);

    auto end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time in milliseconds: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";

    std::cout << "Writing file..." << std::endl;

    write_file("result.jpg", w, h, data);

    stbi_image_free(data);

    return 0;
}
