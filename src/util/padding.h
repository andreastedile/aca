#pragma once

#include <optional>

std::optional<unsigned char*> pad_image(const unsigned char* pixels, int n_rows, const int n_cols, int& new_rows, int& new_cols);
