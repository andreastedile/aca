#ifndef ACA_QUADTREE_LOGGER_H
#define ACA_QUADTREE_LOGGER_H

#include "../rgb.h"

#include <iostream>
#include <string>

class QuadtreeLogger final {
  public:
    explicit QuadtreeLogger(int indent);

    void print_depth(unsigned depth) const;
    void print_i_j(unsigned i, unsigned j) const;
    void print_n_rows_n_cols(unsigned n_rows, unsigned n_cols) const;
    void print_mean(const Pixel& mean) const;
    void print_sq_mean(const RGB<double>& sq_mean) const;
    void print_stdev(const RGB<double>& stdev) const;
    void print_split() const;
    void print_leaf() const;

  private:
    int indent;
};

#endif // ACA_QUADTREE_LOGGER_H
