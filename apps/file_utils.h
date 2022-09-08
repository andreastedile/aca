#ifndef ACA_FILE_UTILS_H
#define ACA_FILE_UTILS_H

#include <cstdint> // uint8_t
#include <string>

uint8_t* read_file(const std::string& filename, int* n_rows, int* n_cols);

void write_file(const std::string& filename, uint8_t* data, int n_rows, int n_cols);

#endif // ACA_FILE_UTILS_H
