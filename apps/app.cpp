#include <chrono>
#include <iostream>

#include "quadtree.hpp"

int main() {
    auto start = std::chrono::steady_clock::now();

    Quadtree root("image.jpg");
    root.build();

    auto end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time in milliseconds: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";

    root.write_to_file("result.jpg");

    return 0;
}
