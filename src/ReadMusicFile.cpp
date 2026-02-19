#include "ReadMusicFile.hpp"

std::vector<float> ReadMusicFile::loadMonoWav(const char* path) const
{
    SF_INFO info{};
    SNDFILE* f = sf_open(path, SFM_READ, &info);
    if (!f) {
        throw std::runtime_error(std::string("sf_open failed: ") + sf_strerror(nullptr));
    }

    if (info.frames <= 0 || info.channels <= 0) {
        sf_close(f);
        throw std::runtime_error("Invalid audio file (frames/channels <= 0)");
    }

    std::vector<float> interleaved(static_cast<std::size_t>(info.frames) * static_cast<std::size_t>(info.channels));
    const sf_count_t got = sf_readf_float(f, interleaved.data(), info.frames);
    sf_close(f);

    if (got <= 0) {
        throw std::runtime_error("sf_readf_float returned no samples");
    }

    const std::size_t frames = static_cast<std::size_t>(got);

    // Hvis allerede mono: return direkte (trim hvis kort read)
    if (info.channels == 1) {
        interleaved.resize(frames);
        return interleaved;
    }

    // Mix til mono
    std::vector<float> mono(frames);
    const int C = info.channels;

    for (std::size_t i = 0; i < frames; ++i) {
        double acc = 0.0;
        const std::size_t base = i * static_cast<std::size_t>(C);
        for (int c = 0; c < C; ++c) acc += interleaved[base + static_cast<std::size_t>(c)];
        mono[i] = static_cast<float>(acc / C);
    }

    return mono;
}
