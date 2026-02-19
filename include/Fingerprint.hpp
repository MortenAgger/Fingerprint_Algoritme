#pragma once
#include <vector>
#include <cstdint>
#include "Peakfinder.hpp"

using FingerprintHash = std::uint32_t;

struct Fingerprint {
    FingerprintHash hash;
    int timeIndex; // anchor time
};

class Fingerprinter {
public:
    std::vector<Fingerprint> generateFingerprints(const std::vector<Peak>& peaks) const;

    // 32-bit packed hash: f1(10) | f2(10) | dt(12)  => 32 bit
    static FingerprintHash makeHash(int f1, int f2, int deltaT);
};

