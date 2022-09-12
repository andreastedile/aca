#pragma once

#include <string>

unsigned char* read_image(const std::string& input, int& n_rows, int& n_cols);

void write_image(unsigned char* pixels, const std::string& output, int n_rows, int n_cols);
