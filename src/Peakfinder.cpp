#include "Peakfinder.hpp"
#include "Config.hpp"
#include <algorithm>

std::vector<Peak> PeakFinder::findPeaks(
    const std::vector<std::vector<double>>& spectrogram,
    int neighborhood,
    double threshold) const
{
    std::vector<Peak> peaks;
    if (spectrogram.empty()) return peaks;

    const int T = static_cast<int>(spectrogram.size());
    const int F = static_cast<int>(spectrogram[0].size());
    if (T <= 2 * neighborhood || F <= 2 * neighborhood) return peaks;

    // 1) Find lokale maxima
    for (int t = neighborhood; t < T - neighborhood; ++t) {
        const auto& row = spectrogram[t];

        for (int f = neighborhood; f < F - neighborhood; ++f) {
            const double mag = row[f];
            if (mag < threshold) continue;

            bool isPeak = true;

            // neighborhood check (tid/frekvens)
            for (int dt = -neighborhood; dt <= neighborhood && isPeak; ++dt) {
                const auto& r = spectrogram[t + dt];
                for (int df = -neighborhood; df <= neighborhood; ++df) {
                    if (dt == 0 && df == 0) continue;
                    if (r[f + df] > mag) { // strengt større = peak
                        isPeak = false;
                        break;
                    }
                }
            }

            if (isPeak) peaks.push_back({t, f, mag});
        }
    }

    if (peaks.empty()) return peaks;

    // 2) Reducér til top-K peaks pr frame (stor performance+DB gevinst)
    // Sortér: time asc, magnitude desc
    std::sort(peaks.begin(), peaks.end(),
        [](const Peak& a, const Peak& b) {
            if (a.timeIndex != b.timeIndex) return a.timeIndex < b.timeIndex;
            return a.magnitude > b.magnitude;
        });

    std::vector<Peak> filtered;
    filtered.reserve(peaks.size());

    int currentT = peaks[0].timeIndex;
    int count = 0;

    for (const auto& p : peaks) {
        if (p.timeIndex != currentT) {
            currentT = p.timeIndex;
            count = 0;
        }
        if (count < MAX_PEAKS_PER_FRAME) {
            filtered.push_back(p);
            ++count;
        }
    }

    return filtered;
}
