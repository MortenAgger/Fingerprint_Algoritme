#include "FFT.hpp"
#include <fftw3.h>
#include <stdexcept>
#include <algorithm>

FFT::FFT(int N_) : N(N_), in(static_cast<std::size_t>(N_)), out(static_cast<std::size_t>(N_ / 2 + 1)), plan(nullptr)
{
    if (N <= 0) {
        throw std::runtime_error("FFT: vinduesstørrelse skal være > 0");
    }

    plan = reinterpret_cast<fftw_plan_s*>(
        fftw_plan_dft_r2c_1d(
            N,
            in.data(),
            reinterpret_cast<fftw_complex*>(out.data()),
            FFTW_MEASURE
        )
    );

    if (!plan) {
        throw std::runtime_error("FFT: kunne ikke oprette plan");
    }
}

FFT::~FFT()
{
    if (plan) {
        fftw_destroy_plan(reinterpret_cast<fftw_plan>(plan));
        plan = nullptr;
    }
}

const std::vector<std::complex<double>>& FFT::compute(const std::vector<double>& input)
{
    if (input.size() != static_cast<std::size_t>(N)) {
        throw std::runtime_error("FFT: input har forkert størrelse");
    }

    std::copy(input.begin(), input.end(), in.begin());
    fftw_execute(reinterpret_cast<fftw_plan>(plan));
    return out;
}
