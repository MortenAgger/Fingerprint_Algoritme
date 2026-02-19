#pragma once
#include <vector>
#include <stdexcept>
#include <string>
#include <sndfile.h>

class ReadMusicFile {
public:
    std::vector<float> loadMonoWav(const char* path) const;
};
