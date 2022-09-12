#include "colorization.h"
#include "construction_host.h"
#include "node.h"
#include "padding.h"
#include "qtmath.h"
#include "rgbaos.h"

#include <argparse/argparse.hpp>
#include <chrono>
#include <fstream>
#include <omp.h>
#include <spdlog/spdlog.h>
#include <stb_image.h>
#include <stb_image_write.h>

using Clock = std::chrono::steady_clock;
using ms = std::chrono::milliseconds;

int main(int argc, char* argv[]) {
    spdlog::set_level(spdlog::level::debug);

    argparse::ArgumentParser app("qt-it-cpu");

    app.add_argument("--input")
        .required()
        .help("specify the input file");
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

    auto input = app.get("--input");
    auto detail_threshold = app.get<float>("--detail-threshold");
    auto no_output_file = app.get<bool>("--no-output-file");
    auto csv = app.get<bool>("--csv");

    spdlog::info("Read {}", input);
    int n_rows, n_cols, n = 0;
    unsigned char* pixels = stbi_load(input.c_str(), &n_cols, &n_rows, &n, 3);
    if (pixels == nullptr) {
        spdlog::error("File {} does not exist", input);
        std::exit(EXIT_FAILURE);
    }
    spdlog::info("Image is {}x{}", n_rows, n_cols);

    if (auto new_pixels = pad_image(pixels, n_rows, n_cols, n_rows, n_cols); new_pixels.has_value()) {
        delete[] pixels;
        pixels = new_pixels.value();
        spdlog::info("Image was padded, now is {}x{}", n_rows, n_cols);
    }

    int n_pixels = n_rows * n_cols;

    spdlog::info("Flatten to RGB AoS");
    auto flatten_start = Clock::now();
    const auto h_aos = flatten_to_rgb_aos(pixels, n_rows, n_cols);
    auto flatten_end = Clock::now();

    int tree_height = log4(n_pixels);
    int n_nodes = (pow_base4(tree_height + 1) - 1) / 3;

    spdlog::info("Allocate quadtree array on host (size in bytes: {})", (size_t)n_nodes * (sizeof(Node)));
    auto allocate_on_host_start = Clock::now();
    Node* h_quadtree_array = new Node[n_nodes];
    auto allocate_on_host_end = Clock::now();

    omp_set_num_threads(8);
    spdlog::info("Init leaves on host");
    auto init_leaves_on_host_start = Clock::now();
    init_quadtree_array_leaves_host(h_quadtree_array, tree_height, h_aos);
    auto init_leaves_on_host_end = Clock::now();

    int from_level = tree_height - 1;
    spdlog::info("Construct quadtree array on host from level {}", from_level);
    auto construct_on_host_start = Clock::now();
    construct_quadtree_array_host(h_quadtree_array, from_level, detail_threshold);
    auto construct_on_host_end = Clock::now();

    if (csv) {
        std::ofstream file("timings.csv");
        file << "flatten_ms, allocate_on_host_ms, init_leaves_host, construct_on_host_ms\n";
        file << std::chrono::duration_cast<ms>(flatten_end - flatten_start).count() << ", ";
        file << std::chrono::duration_cast<ms>(allocate_on_host_end - allocate_on_host_start).count() << ", ";
        file << std::chrono::duration_cast<ms>(init_leaves_on_host_end - init_leaves_on_host_start).count() << ", ";
        file << std::chrono::duration_cast<ms>(construct_on_host_end - construct_on_host_start).count() << '\n';
    }

    if (!no_output_file) {
        for (int i = 0; i <= tree_height; i++) {
            spdlog::info("Colorize level {}", i);
            colorize(pixels, n_rows, n_cols, h_quadtree_array, i);

            spdlog::info("Write image");
            auto output = std::string("level") + std::to_string(i) + std::string(".jpg");
            stbi_write_jpg(output.c_str(), n_cols, n_rows, 3, pixels, 100);
        }
    }

    delete[] h_aos.aos;

    delete[] h_quadtree_array;
    delete[] pixels;

    spdlog::info("All done");

    return 0;
}
