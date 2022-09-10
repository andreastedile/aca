#include "bottom_up.h"
#include "colorization.h"
#include "padding.h"
#include "quadrant.h"
#include "rgbsoa.h"
#include "top_down.h"

#include <argparse/argparse.hpp>
#include <fstream>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>
#include <stb_image.h>
#include <stb_image_write.h>

int main(int argc, char* argv[]) {
    spdlog::set_level(spdlog::level::debug);

    argparse::ArgumentParser app("app");

    app.add_argument("--input")
        .required()
        .help("specify the input file");
    app.add_argument("--top-down")
        .default_value(false)
        .implicit_value(true)
        .help("specify whether to execute the top-down algorithm instead of the default bottom-up one");
    app.add_argument("--detail-threshold")
        .scan<'g', double>()
        .default_value(13.0)
        .help("specify the detail threshold");
    app.add_argument("--no-output-file")
        .default_value(false)
        .implicit_value(true)
        .help("suppress the production of the resulting image");

    app.parse_args(argc, argv);

    auto input = app.get("input");
    auto do_top_down = app.get<bool>("--top-down");
    auto detail_threshold = app.get<double>("--detail-threshold");
    auto no_output_file = app.get<bool>("--no-output-file");

    std::ofstream csv("timings.csv");
    csv << "flatten_ms, construction_ms\n";

    spdlog::stopwatch sw;

    spdlog::info("Read {}", input);
    sw.reset();
    int n_rows, n_cols, n;
    unsigned char* pixels = stbi_load(input.c_str(), &n_cols, &n_rows, &n, 3);
    spdlog::info("Read took {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(sw.elapsed()).count());
    spdlog::info("Image is {}x{}", n_rows, n_cols);

    if (auto new_pixels = pad_image(pixels, n_rows, n_cols, n_rows, n_cols); new_pixels.has_value()) {
        delete[] pixels;
        pixels = new_pixels.value();
        spdlog::info("Image was padded, now is {}x{}", n_rows, n_cols);
    }

    spdlog::info("Flatten to RGB SoA");
    sw.reset();
    const auto soa = flatten_to_rgb_soa(pixels, n_rows, n_cols);
    auto elapsed = sw.elapsed();
    spdlog::info("Flatten to RGB SoA took {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count());
    csv << elapsed.count() << ", ";

    auto quadrant = std::make_unique<Quadrant>(0, 0, n_rows, n_cols, soa);
    std::unique_ptr<Quadtree> quadtree_root;
    if (do_top_down) {
        spdlog::info("Build quadtree top down");
        sw.reset();
        quadtree_root = top_down(std::move(quadrant), detail_threshold);
    } else {
        spdlog::info("Build quadtree bottom up");
        sw.reset();
        quadtree_root = bottom_up(std::move(quadrant), detail_threshold);
    }
    elapsed = sw.elapsed();
    spdlog::info("Build quadtree took {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count());
    csv << elapsed.count() << '\n';

    if (!no_output_file) {
        spdlog::info("Colorize");
        sw.reset();
        colorize(pixels, n_rows, n_cols, *quadtree_root);
        spdlog::info("Colorize took {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(sw.elapsed()).count());

        spdlog::info("Write image");
        sw.reset();
        stbi_write_jpg("result.jpg", n_cols, n_rows, 3, pixels, 100);
        spdlog::info("Write image took {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(sw.elapsed()).count());
    }

    delete[] pixels;

    spdlog::info("All done");

    return 0;
}
