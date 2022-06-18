#include "quadtree_logger.h"

#include <iostream>
#include <string>

QuadtreeLogger::QuadtreeLogger(int indent) : indent(indent) {}

void QuadtreeLogger::print_depth(unsigned depth) const {
    std::cout << std::string(indent, ' ') << "depth: " << depth << '\n';
}
void QuadtreeLogger::print_i_j(unsigned i, unsigned j) const {
    std::cout << std::string(indent, ' ') << "i: " << i << ", j: " << j << '\n';
}
void QuadtreeLogger::print_n_rows_n_cols(unsigned n_rows, unsigned n_cols) const {
    std::cout << std::string(indent, ' ') << "rows: " << n_rows << ", cols: " << n_cols << '\n';
}
void QuadtreeLogger::print_mean(const Pixel& mean) const {
    std::cout << std::string(indent, ' ') << "mean: " << +mean.r << ' ' << +mean.g << ' ' << +mean.b << '\n';
}
void QuadtreeLogger::print_sq_mean(const RGB<double>& sq_mean) const {
    std::cout << std::string(indent, ' ') << "sq_mean: " << sq_mean.r << ' ' << sq_mean.g << ' ' << sq_mean.b << '\n';
}
void QuadtreeLogger::print_stdev(const RGB<double>& stdev) const {
    std::cout << std::string(indent, ' ') << "stdev: " << stdev.r << ' ' << stdev.g << ' ' << stdev.b << '\n';
}
void QuadtreeLogger::print_split() const {
    std::cout << std::string(indent, ' ') << "split\n";
}
void QuadtreeLogger::print_leaf() const {
    std::cout << std::string(indent, ' ') << "leaf\n";
}
