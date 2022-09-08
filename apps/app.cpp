#include "colorizer/colorizer.h"
#include "eigen_quadrant/eigen_quadrant.h"
#include "eigen_soa/eigen_soa.h"
#include "file_utils.h"
#include "quadtree/top_down.h"

#if defined(TIME_READ_FILE) || defined(TIME_FLATTEN) || defined(TIME_QUADTREE_BUILD) || defined(TIME_COLORIZE) || defined(TIME_WRITE_FILE)
#include <chrono>
#endif
#include <iostream>
#include <memory>

#define MAX_DEPTH 8
#define DETAIL_THRESHOLD 13

int main(int, char* argv[]) {
    std::cout << "Reading " << argv[1] << "...\n";
#ifdef TIME_READ_FILE
    auto read_start = std::chrono::steady_clock::now();
#endif
    int n_rows, n_cols;
    auto* pixels = read_file(argv[1], &n_rows, &n_cols);
#ifdef TIME_READ_FILE
    auto read_end = std::chrono::steady_clock::now();
    std::cout << "Read file took "
              << std::chrono::duration_cast<std::chrono::milliseconds>(read_end - read_start).count()
              << " ms\n";
#endif

    std::cout << "Computing soa...\n";
#ifdef TIME_SOA
    auto soa_start = std::chrono::steady_clock::now();
#endif
    const auto soa = to_eigen_pixel_soa(pixels, n_rows * n_cols);
#ifdef TIME_SOA
    auto soa_end = std::chrono::steady_clock::now();
    std::cout << "Soa took "
              << std::chrono::duration_cast<std::chrono::milliseconds>(soa_end - soa_start).count()
              << " ms\n";
#endif

    std::cout << "Building quadtree...\n";
#ifdef TIME_QUADTREE_BUILD
    auto build_start = std::chrono::steady_clock::now();
#endif
    auto quadrant = std::make_unique<EigenQuadrant>(0, 0, n_rows, n_cols, soa);
    auto root = top_down(std::move(quadrant), DETAIL_THRESHOLD, MAX_DEPTH);
#ifdef TIME_QUADTREE_BUILD
    auto build_end = std::chrono::steady_clock::now();
    std::cout << "Quadtree build took "
              << std::chrono::duration_cast<std::chrono::milliseconds>(build_end - build_start).count()
              << " ms\n";
#endif

    std::cout << "Coloring...\n";
#ifdef TIME_COLORIZE
    auto colorize_start = std::chrono::steady_clock::now();
#endif
    colorize(pixels, n_rows, n_cols, *root);
#ifdef TIME_COLORIZE
    auto colorize_end = std::chrono::steady_clock::now();
    std::cout << "Colorize took "
              << std::chrono::duration_cast<std::chrono::milliseconds>(colorize_end - colorize_start).count()
              << " ms\n";
#endif

    std::cout << "Writing result...\n";
#ifdef TIME_WRITE_FILE
    auto write_start = std::chrono::steady_clock::now();
#endif
    write_file("result.jpg", pixels, n_rows, n_cols);
#ifdef TIME_WRITE_FILE
    auto write_end = std::chrono::steady_clock::now();
    std::cout << "Write took "
              << std::chrono::duration_cast<std::chrono::milliseconds>(write_end - write_start).count()
              << " ms\n";
#endif

    delete[] pixels;

    return 0;
}
