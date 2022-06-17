#ifndef ACA_PRINTER_H
#define ACA_PRINTER_H

#include <iostream>
#include <string>

using color_t = uint8_t;

struct Printer {
    unsigned indent;

    explicit Printer(unsigned indent) : indent(indent) {}

    void print_depth(unsigned depth) const {
        std::cout << std::string(indent, ' ') << "depth: " << depth << '\n';
    }
    void print_i_j(unsigned i, unsigned j) const {
        std::cout << std::string(indent, ' ') << "i: " << i << ", j: " << j << '\n';
    }
    void print_n_rows_n_cols(unsigned n_rows, unsigned n_cols) const {
        std::cout << std::string(indent, ' ') << "rows: " << n_rows << ", cols: " << n_cols << '\n';
    }
    void print_mean(double r, double g, double b) const {
        std::cout << std::string(indent, ' ') << "mean: " << +r << ' ' << +g << ' ' << +b << '\n';
    }
    void print_sq_mean(double r, double g, double b) const {
        std::cout << std::string(indent, ' ') << "sq_mean: " << r << ' ' << g << ' ' << b << '\n';
    }
    void print_stdev(double r, double g, double b) const {
        std::cout << std::string(indent, ' ') << "stdev: " << r << ' ' << g << ' ' << b << '\n';
    }
    void print_split() const {
        std::cout << std::string(indent, ' ') << "split\n";
    }
    void print_leaf() const {
        std::cout << std::string(indent, ' ') << "leaf\n";
    }
};

#endif // ACA_PRINTER_H
