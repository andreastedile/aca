#include "file_utils.h"

#include <stdexcept> // runtime_error

// https://github.com/nothings/stb/blob/master/stb_image.h#L12
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
// https://github.com/nothings/stb/blob/master/stb_image_write.h#L7
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

uint8_t* read_file(const std::string& filename, int* n_rows, int* n_cols) {
    int n;
    uint8_t* data = stbi_load(filename.c_str(), n_cols, n_rows, &n, 3);
    if (!data) {
        throw std::runtime_error("Could not open file " + filename);
    }
    return data;
}

void write_file(const std::string& filename, uint8_t* data, int n_rows, int n_cols) {
    stbi_write_jpg(filename.c_str(), n_cols, n_rows, 3, data, 100);
}
