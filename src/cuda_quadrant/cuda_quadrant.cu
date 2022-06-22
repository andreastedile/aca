#include "cuda_quadrant.cuh"

#include <thrust/reduce.h>
#include <thrust/transform_reduce.h>

CudaQuadrant::CudaQuadrant(int i, int j, int n_rows, int n_cols, const CudaDevicePixelSoa& soa) : CudaQuadrant(i, j, n_rows, n_cols, soa, 0, n_rows * n_cols) {}

CudaQuadrant::CudaQuadrant(int i, int j, int n_rows, int n_cols, const CudaDevicePixelSoa& soa, int left, int length)
    : Quadrant(i, j, n_rows, n_cols), soa(soa), left(left), length(length) {}

struct square {
    __device__ double operator()(const uint8_t& x) const {
        return double(x) * double(x);
    }
};

RGB<double> CudaQuadrant::mean() const {
    auto right = left + length;
    return {thrust::reduce(soa.r.begin() + left, soa.r.begin() + right, (double)0) / length,
            thrust::reduce(soa.g.begin() + left, soa.g.begin() + right, (double)0) / length,
            thrust::reduce(soa.b.begin() + left, soa.b.begin() + right, (double)0) / length};
}

RGB<double> CudaQuadrant::sq_mean() const {
    auto right = left + length;
    return {thrust::transform_reduce(soa.r.begin() + left, soa.r.begin() + right, square(), (double)0, thrust::plus<double>()) / length,
            thrust::transform_reduce(soa.g.begin() + left, soa.g.begin() + right, square(), (double)0, thrust::plus<double>()) / length,
            thrust::transform_reduce(soa.b.begin() + left, soa.b.begin() + right, square(), (double)0, thrust::plus<double>()) / length};
}

std::unique_ptr<Quadrant> CudaQuadrant::nw() const {
    return std::make_unique<CudaQuadrant>(i + 0, j + 0,
                                          n_rows / 2, n_cols / 2,
                                          soa, left + 0 * (length / 4), length / 4);
}

std::unique_ptr<Quadrant> CudaQuadrant::ne() const {
    return std::make_unique<CudaQuadrant>(i + 0, j + n_cols / 2,
                                          n_rows / 2, n_cols / 2,
                                          soa, left + 1 * (length / 4), length / 4);
}

std::unique_ptr<Quadrant> CudaQuadrant::se() const {
    return std::make_unique<CudaQuadrant>(i + n_rows / 2, j + 0,
                                          n_rows / 2, n_cols / 2,
                                          soa, left + 2 * (length / 4), length / 4);
}

std::unique_ptr<Quadrant> CudaQuadrant::sw() const {
    return std::make_unique<CudaQuadrant>(i + n_rows / 2, j + n_cols / 2,
                                          n_rows / 2, n_cols / 2,
                                          soa, left + 3 * (length / 4), length / 4);
}
