#pragma once
#include <unordered_map>
#include <vector>
#include <cstdint>
#include "Fingerprint.hpp"

struct DBEntry {
    std::uint16_t songId;
    std::uint32_t timeIndex;
};

using FingerprintDB = std::unordered_map<FingerprintHash, std::vector<DBEntry>>;

void addSongToDatabase(
    FingerprintDB& db,
    const std::vector<Fingerprint>& prints,
    int songId
);

int identifySong(
    const FingerprintDB& db,
    const std::vector<Fingerprint>& queryPrints
);
