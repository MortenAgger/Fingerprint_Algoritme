#include "FingerprintDB.hpp"
#include "Config.hpp"
#include <unordered_map>
#include <cstdint>
#include <algorithm>

void addSongToDatabase(
    FingerprintDB& db,
    const std::vector<Fingerprint>& prints,
    int songId)
{
    const std::uint16_t sid = static_cast<std::uint16_t>(songId);

    // (Valgfrit) lille reserve-hjælp: groft anslå antal buckets
    if (db.empty()) db.reserve(prints.size() / 4 + 1);

    for (const auto& fp : prints) {
        db[fp.hash].push_back(DBEntry{sid, static_cast<std::uint32_t>(fp.timeIndex)});
    }
}

int identifySong(
    const FingerprintDB& db,
    const std::vector<Fingerprint>& queryPrints)
{
    if (queryPrints.empty()) return 0;

    // votes på (songId, offset) – offset kan være negativ, gem som uint32 bits
    std::unordered_map<std::uint64_t, std::uint16_t> votes;
    votes.reserve(queryPrints.size() * 6);
    votes.max_load_factor(0.7f);

    // bedste offset-score per sang
    std::unordered_map<std::uint16_t, int> bestBySong;
    bestBySong.reserve(16);

    int bestSong = 0;
    int bestScore = 0;

    for (const auto& q : queryPrints) {
        const auto it = db.find(q.hash);
        if (it == db.end()) continue;

        for (const auto& entry : it->second) {
            const int delta = static_cast<int>(entry.timeIndex) - q.timeIndex;

            const std::uint64_t key =
                (static_cast<std::uint64_t>(entry.songId) << 32) |
                (static_cast<std::uint32_t>(delta)); // bevar bitmønster

            const int score = ++votes[key];

            auto& songBest = bestBySong[entry.songId];
            if (score > songBest) songBest = score;

            if (songBest > bestScore) {
                bestScore = songBest;
                bestSong = static_cast<int>(entry.songId);
            }
        }
    }

    if (bestScore < MIN_SCORE) return 0;
    return bestSong;
}
