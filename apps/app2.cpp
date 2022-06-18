#if defined(TIME_READ_FILE) || defined(TIME_FLATTEN) || defined(TIME_QUADTREE_BUILD) || defined(TIME_COLORIZE) || defined(TIME_WRITE_FILE)
#include <chrono>
#endif

#include <iostream>

#include "colorizer.h"
#include "file_utils.h"
#include "flatten.h"
#include "quadtree_with_eigen.h"

int main(int, char* argv[]) {
    std::cout << "Reading " << argv[1] << "...\n";
#ifdef TIME_READ_FILE
    auto read_start = std::chrono::steady_clock::now();
#endif
    int n_rows, n_cols;
    auto* data = read_file(argv[1], &n_rows, &n_cols);
#ifdef TIME_READ_FILE
    auto read_end = std::chrono::steady_clock::now();
    std::cout << "Read file took "
              << std::chrono::duration_cast<std::chrono::milliseconds>(read_end - read_start).count()
              << " ms\n";
#endif

    std::cout << "Flattening...\n";
#ifdef TIME_FLATTEN
    auto flatten_start = std::chrono::steady_clock::now();
#endif
    auto soa = flatten_data(data, n_rows, n_cols);
#ifdef TIME_FLATTEN
    auto flatten_end = std::chrono::steady_clock::now();
    std::cout << "Flatten took "
              << std::chrono::duration_cast<std::chrono::milliseconds>(flatten_end - flatten_start).count()
              << " ms\n";
#endif

    std::cout << "Building quadtree...\n";
#ifdef TIME_QUADTREE_BUILD
    auto build_start = std::chrono::steady_clock::now();
#endif
    Quadtree root(n_rows, n_cols);
    root.build(soa, 0, n_rows * n_cols);
#ifdef TIME_QUADTREE_BUILD
    auto build_end = std::chrono::steady_clock::now();
    std::cout << "Flatten took "
              << std::chrono::duration_cast<std::chrono::milliseconds>(build_end - build_start).count()
              << " ms\n";
#endif

    std::cout << "Coloring...\n";
#ifdef TIME_COLORIZE
    auto colorize_start = std::chrono::steady_clock::now();
#endif
    colorize(data, root);
#ifdef TIME_QUADTREE_BUILD
    auto colorize_end = std::chrono::steady_clock::now();
    std::cout << "Colorize took "
              << std::chrono::duration_cast<std::chrono::milliseconds>(colorize_end - colorize_start).count()
              << " ms\n";
#endif

    std::cout << "Writing result...\n";
#ifdef TIME_WRITE_FILE
    auto write_start = std::chrono::steady_clock::now();
#endif
    write_file("result.jpg", data, n_rows, n_cols);
#ifdef TIME_WRITE_FILE
    auto write_end = std::chrono::steady_clock::now();
    std::cout << "Write took "
              << std::chrono::duration_cast<std::chrono::milliseconds>(write_end - write_start).count()
              << " ms\n";
#endif

    stbi_image_free(data);

    return 0;
}
