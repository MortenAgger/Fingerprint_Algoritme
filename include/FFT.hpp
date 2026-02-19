#pragma once
#include <vector>
#include <complex>
#include <cstddef>

struct fftw_plan_s; // forward-decl (FFTW type)

class FFT {
public:
    explicit FFT(int N);
    ~FFT();

    FFT(const FFT&)            = delete;
    FFT& operator=(const FFT&) = delete;

    int size() const { return N; }
    std::size_t bins() const { return out.size(); }

    // Returnerer reference for at undgå kopi pr frame
    const std::vector<std::complex<double>>& compute(const std::vector<double>& input);

private:
    int N;
    std::vector<double> in;
    std::vector<std::complex<double>> out;
    fftw_plan_s* plan; // opak pointer (plan)
};
