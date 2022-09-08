#include "colorizer/colorizer.h"
#include "eigen_quadrant/eigen_quadrant.h"
#include "eigen_soa/eigen_soa.h"
#include "quadtree/bottom_up.h"
#include "quadtree/top_down.h"

#include <argparse/argparse.hpp>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>
#include <stb_image.h>
#include <stb_image_write.h>

int main(int argc, char* argv[]) {
    argparse::ArgumentParser app("app");

    app.add_argument("input")
        .required()
        .help("specify the input file");
    app.add_argument("--top-down")
        .default_value(false)
        .implicit_value(true)
        .help("specify whether to execute the top-down algorithm instead of the default bottom-up one");
    app.add_argument("--max-depth")
        .scan<'d', int>()
        .default_value(8.0)
        .help("specify the max depth");
    app.add_argument("--detail-threshold")
        .scan<'g', double>()
        .default_value(13.0)
        .help("specify the detail threshold");

    app.parse_args(argc, argv);

    auto input = app.get("input");
    auto do_top_down = app.get<bool>("--top-down");
    auto max_depth = app.get<int>("--max-depth");
    auto detail_threshold = app.get<double>("--detail-threshold");

    spdlog::stopwatch sw;

    spdlog::info("Read {}", argv[1]);
    int n_rows, n_cols, n;
    uint8_t* pixels = stbi_load(input.c_str(), &n_cols, &n_rows, &n, 3);

    spdlog::info("Compute SoA");
    sw.reset();
    const auto soa = to_eigen_pixel_soa(pixels, n_rows * n_cols);
    spdlog::info("Compute SoA took {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(sw.elapsed()).count());

    auto quadrant = std::make_unique<EigenQuadrant>(0, 0, n_rows, n_cols, soa);
    std::unique_ptr<Quadtree> root;
    if (do_top_down) {
        spdlog::info("Build quadtree top down");
        sw.reset();
        root = top_down(std::move(quadrant), detail_threshold, max_depth);
    } else {
        spdlog::info("Build quadtree bottom up");
        sw.reset();
        root = bottom_up(std::move(quadrant), detail_threshold, max_depth);
    }
    spdlog::info("Build quadtree took {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(sw.elapsed()).count());

    spdlog::info("Colorize");
    sw.reset();
    colorize(pixels, n_rows, n_cols, *root);
    spdlog::info("Colorize took {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(sw.elapsed()).count());

    spdlog::info("Write image");
    stbi_write_jpg("result.jpg", n_cols, n_rows, 3, pixels, 100);

    delete[] pixels;

    spdlog::info("All done");

    return 0;
}
