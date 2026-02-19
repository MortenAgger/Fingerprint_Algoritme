#include "Fingerprint.hpp"
#include "Config.hpp"
#include <algorithm>
#include <vector>
#include <cstdint>

std::vector<Fingerprint> Fingerprinter::generateFingerprints(const std::vector<Peak>& peaks) const
{
    std::vector<Fingerprint> fingerprints;
    if (peaks.empty()) return fingerprints;

    // Sørg for sortering efter tid og magnitude (stabilt fan-out)
    std::vector<Peak> sorted = peaks;
    std::sort(sorted.begin(), sorted.end(),
        [](const Peak& a, const Peak& b) {
            if (a.timeIndex != b.timeIndex) return a.timeIndex < b.timeIndex;
            return a.magnitude > b.magnitude;
        });

    const int maxT = sorted.back().timeIndex;
    if (maxT < 0) return fingerprints;

    // Bucket peaks per frame
    std::vector<std::vector<const Peak*>> byTime(static_cast<std::size_t>(maxT + 1));
    for (const auto& p : sorted) {
        if (p.timeIndex >= 0)
            byTime[static_cast<std::size_t>(p.timeIndex)].push_back(&p);
    }

    // Estimat: anchors ~ (frames * anchors_per_frame) * FAN_OUT
    std::size_t est = 0;
    est = static_cast<std::size_t>(maxT + 1) * static_cast<std::size_t>(MAX_ANCHORS_PER_FRAME) * static_cast<std::size_t>(FAN_OUT);
    fingerprints.reserve(est);

    for (int t = 0; t <= maxT; ++t) {
        auto& anchors = byTime[static_cast<std::size_t>(t)];
        if (anchors.empty()) continue;

        // anchors er allerede i magnitude-desc pga. sorteringen
        const int numAnchors = std::min<int>(MAX_ANCHORS_PER_FRAME, static_cast<int>(anchors.size()));

        for (int ai = 0; ai < numAnchors; ++ai) {
            const Peak* A = anchors[static_cast<std::size_t>(ai)];

            int used = 0;

            // Kig frem i tid (t+1 .. t+MAX_DT-1)
            const int tMax = std::min(maxT, t + (MAX_DT - 1));

            for (int tt = t + 1; tt <= tMax && used < FAN_OUT; ++tt) {
                const auto& targets = byTime[static_cast<std::size_t>(tt)];
                if (targets.empty()) continue;

                // Tag de stærkeste targets først for stabilitet
                // (targets er også magnitude-desc pr frame)
                for (std::size_t bi = 0; bi < targets.size() && used < FAN_OUT; ++bi) {
                    const Peak* B = targets[bi];
                    const int dt = tt - t;
                    // dt > 0 her altid, og dt < MAX_DT pga. loop

                    const FingerprintHash h = makeHash(A->freqIndex, B->freqIndex, dt);
                    fingerprints.push_back({h, t});
                    ++used;
                }
            }
        }
    }

    return fingerprints;
}

FingerprintHash Fingerprinter::makeHash(int f1, int f2, int deltaT)
{
    // Pakning: 10 bit f1, 10 bit f2, 12 bit dt
    // Hvis dine bins er >1023, øg bitfordelingen (og skift til 64-bit igen).
    const std::uint32_t a = static_cast<std::uint32_t>(f1) & 0x3FFu;
    const std::uint32_t b = static_cast<std::uint32_t>(f2) & 0x3FFu;
    const std::uint32_t dt = static_cast<std::uint32_t>(deltaT) & 0xFFFu;

    return (a << 22) | (b << 12) | dt;
}
