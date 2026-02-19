#pragma once
#include <vector>

class Spectrogram {
public:
    std::vector<std::vector<double>> computeSpectrogram(
        const std::vector<float>& signal,
        int windowSize,
        int hopSize
    ) const;
};
