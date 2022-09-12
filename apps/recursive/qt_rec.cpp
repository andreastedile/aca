#include "bottom_up.h"
#include "colorization.h"
#include "io.h"
#include "padding.h"
#include "quadrant.h"
#include "rgbsoa.h"
#include "top_down.h"

#include <argparse/argparse.hpp>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <spdlog/spdlog.h>

using Clock = std::chrono::steady_clock;
using ms = std::chrono::milliseconds;

int main(int argc, char* argv[]) {
    spdlog::set_level(spdlog::level::debug);

    argparse::ArgumentParser app("qt-rec");

    app.add_argument("--input")
        .required()
        .help("specify the input file");
    app.add_argument("--top-down")
        .default_value(false)
        .implicit_value(true)
        .help("specify whether to execute the top-down algorithm instead of the default bottom-up one");
    app.add_argument("--detail-threshold")
        .scan<'g', float>()
        .default_value(13.0f)
        .help("specify the detail threshold");
    app.add_argument("--no-output-file")
        .default_value(false)
        .implicit_value(true)
        .help("suppress the production of the resulting image");
    app.add_argument("--csv")
        .default_value(false)
        .implicit_value(true)
        .help("writes timing information to csv file");

    app.parse_args(argc, argv);

    auto input = app.get("input");
    auto do_top_down = app.get<bool>("--top-down");
    auto detail_threshold = app.get<float>("--detail-threshold");
    auto no_output_file = app.get<bool>("--no-output-file");
    auto csv = app.get<bool>("--csv");

    spdlog::info("Read {}", input);
    int n_rows, n_cols, n = 0;
    unsigned char* pixels = read_image(input, n_rows, n_cols);

    spdlog::info("Flatten to RGB SoA");
    auto flatten_start = Clock::now();
    const auto soa = flatten_to_rgb_soa(pixels, n_rows, n_cols);
    auto flatten_end = Clock::now();

    auto construct_start = Clock::now();
    auto quadrant = std::make_unique<Quadrant>(0, 0, n_rows, n_cols, soa);
    std::unique_ptr<Quadtree> quadtree_root;
    if (do_top_down) {
        spdlog::info("Build quadtree top down");
        quadtree_root = top_down(std::move(quadrant), detail_threshold);
    } else {
        spdlog::info("Build quadtree bottom up");
        quadtree_root = bottom_up(std::move(quadrant), detail_threshold);
    }
    auto construct_end = Clock::now();

    if (csv) {
        std::ofstream file("timings.csv");
        file << "flatten_ms, construct_ms\n";
        file << std::chrono::duration_cast<ms>(flatten_end - flatten_start).count() << ", ";
        file << std::chrono::duration_cast<ms>(construct_end - construct_start).count() << '\n';
    }

    if (!no_output_file) {
        spdlog::info("Colorize");
        colorize(pixels, n_rows, n_cols, *quadtree_root);

        spdlog::info("Write image");
        write_image(pixels, "result.jpg", n_rows, n_cols);
    }

    delete[] pixels;

    spdlog::info("All done");

    return 0;
}
