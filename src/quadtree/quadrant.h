#ifndef ACA_QUADRANT_H
#define ACA_QUADRANT_H

#include "rgb.h"

#include <cassert>
#include <memory>

template <class T>
class Quadrant {
  public:
    Quadrant(int i,
             int j,
             int n_rows,
             int n_cols) : i(i), j(j), n_rows(n_rows), n_cols(n_cols) {
        assert(i >= 0);
        assert(j >= 0);
        assert(n_rows >= 1);
        assert(n_cols >= 1);
    }
    [[nodiscard]] RGB<double> mean() const { return static_cast<const T*>(this)->mean_impl(); };
    [[nodiscard]] RGB<double> sq_mean() const { return static_cast<const T*>(this)-> sq_mean_impl(); };

    [[nodiscard]] T nw() const { return static_cast<const T*>(this)->nw_impl(); }
    [[nodiscard]] T ne() const { return static_cast<const T*>(this)->ne_impl(); }
    [[nodiscard]] T se() const { return static_cast<const T*>(this)->se_impl(); }
    [[nodiscard]] T sw() const { return static_cast<const T*>(this)->sw_impl(); }

    int i;
    int j;
    int n_rows;
    int n_cols;
};

#endif // ACA_QUADRANT_H
