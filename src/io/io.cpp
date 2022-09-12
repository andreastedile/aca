#include "io.h"
#include "padding.h"

#include <spdlog/spdlog.h>
#include <stb_image.h>
#include <stb_image_write.h>

unsigned char* read_image(const std::string& input, int& n_rows, int& n_cols) {
    int n = 0;
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
    return pixels;
}

void write_image(unsigned char* pixels, const std::string& output, int n_rows, int n_cols) {
    stbi_write_jpg(output.c_str(), n_cols, n_rows, 3, pixels, 100);
}
