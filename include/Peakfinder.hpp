#pragma once
#include <vector>

struct Peak {
    int timeIndex;
    int freqIndex;
    double magnitude;
};

class PeakFinder {
public:
    std::vector<Peak> findPeaks(
        const std::vector<std::vector<double>>& spectrogram,
        int neighborhood,
        double threshold
    ) const;
};
