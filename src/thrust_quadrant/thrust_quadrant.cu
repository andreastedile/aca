#include "thrust_quadrant.cuh"

#include <thrust/reduce.h>
#include <thrust/transform_reduce.h>

ThrustQuadrant::ThrustQuadrant(int i, int j, int n_rows, int n_cols, const ThrustDevicePixelSoa& soa) : ThrustQuadrant(i, j, n_rows, n_cols, soa, 0, n_rows * n_cols) {}

ThrustQuadrant::ThrustQuadrant(int i, int j, int n_rows, int n_cols, const ThrustDevicePixelSoa& soa, int left, int length)
    : Quadrant(i, j, n_rows, n_cols), soa(soa), left(left), length(length) {}

struct square {
    __device__ double operator()(const uint8_t& x) const {
        return double(x) * double(x);
    }
};

RGB<double> ThrustQuadrant::mean_impl() const {
    auto right = left + length;
    return {thrust::reduce(soa.r.begin() + left, soa.r.begin() + right, (double)0) / length,
            thrust::reduce(soa.g.begin() + left, soa.g.begin() + right, (double)0) / length,
            thrust::reduce(soa.b.begin() + left, soa.b.begin() + right, (double)0) / length};
}

RGB<double> ThrustQuadrant::sq_mean_impl() const {
    auto right = left + length;
    return {thrust::transform_reduce(soa.r.begin() + left, soa.r.begin() + right, square(), (double)0, thrust::plus<double>()) / length,
            thrust::transform_reduce(soa.g.begin() + left, soa.g.begin() + right, square(), (double)0, thrust::plus<double>()) / length,
            thrust::transform_reduce(soa.b.begin() + left, soa.b.begin() + right, square(), (double)0, thrust::plus<double>()) / length};
}

ThrustQuadrant ThrustQuadrant::nw_impl() const {
    return {i + 0, j + 0,
            n_rows / 2, n_cols / 2,
            soa, left + 0 * (length / 4), length / 4};
}

ThrustQuadrant ThrustQuadrant::ne_impl() const {
    return {i + 0, j + n_cols / 2,
            n_rows / 2, n_cols / 2,
            soa, left + 1 * (length / 4), length / 4};
}

ThrustQuadrant ThrustQuadrant::se_impl() const {
    return {i + n_rows / 2, j + 0,
            n_rows / 2, n_cols / 2,
            soa, left + 2 * (length / 4), length / 4};
}

ThrustQuadrant ThrustQuadrant::sw_impl() const {
    return {i + n_rows / 2, j + n_cols / 2,
            n_rows / 2, n_cols / 2,
            soa, left + 3 * (length / 4), length / 4};
}
