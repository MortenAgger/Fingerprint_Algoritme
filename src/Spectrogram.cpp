#include "Spectrogram.hpp"
#include "FFT.hpp"
#include <cmath>
#include <algorithm>

static inline double pi() { return std::acos(-1.0); }

std::vector<std::vector<double>> Spectrogram::computeSpectrogram(
    const std::vector<float>& signal, int windowSize, int hopSize) const
{
    std::vector<std::vector<double>> spectrogram;

    if (windowSize <= 0 || hopSize <= 0) return spectrogram;
    if (signal.size() < static_cast<std::size_t>(windowSize)) return spectrogram;

    const int numWindows = static_cast<int>((signal.size() - windowSize) / hopSize) + 1;
    spectrogram.reserve(numWindows);

    FFT fft(windowSize);

    // Precompute Hann
    std::vector<double> hann(static_cast<std::size_t>(windowSize));
    const double twoPi = 2.0 * pi();
    for (int i = 0; i < windowSize; ++i) {
        hann[static_cast<std::size_t>(i)] = 0.5 * (1.0 - std::cos(twoPi * i / (windowSize - 1)));
    }

    // Genbrug buffers (ingen allok pr frame)
    std::vector<double> window(static_cast<std::size_t>(windowSize));
    std::vector<double> magnitudes(fft.bins());

    // dB fra POWER: 10*log10(re^2+im^2)
    constexpr double EPS = 1e-24;

    for (int w = 0; w < numWindows; ++w) {
        const int start = w * hopSize;
        if (start + windowSize > static_cast<int>(signal.size())) break;

        for (int i = 0; i < windowSize; ++i) {
            window[static_cast<std::size_t>(i)] =
                static_cast<double>(signal[static_cast<std::size_t>(start + i)]) * hann[static_cast<std::size_t>(i)];
        }

        const auto& X = fft.compute(window);

        for (std::size_t k = 0; k < X.size(); ++k) {
            const double re = X[k].real();
            const double im = X[k].imag();
            const double power = re * re + im * im;
            magnitudes[k] = 10.0 * std::log10(power + EPS);
        }

        spectrogram.push_back(magnitudes); // kopierer kun 1 vektor pr frame (nødvendigt pga. output)
    }

    return spectrogram;
}
