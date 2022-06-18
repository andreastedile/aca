#include <chrono>
#include <iostream>

#include "old_quadtree/old_quadtree.hpp"

int main(int, char* argv[]) {
    auto start = std::chrono::steady_clock::now();

    OldQuadtree root(argv[1]);
    root.build();

    auto end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time in milliseconds: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";

    root.write_to_file("result.jpg");

    return 0;
}
