#pragma once

// Spectrogram
constexpr int WINDOW_SIZE = 4096;
constexpr int HOP_SIZE    = 1024;

// Peak finding
constexpr int    NEIGHBORHOOD = 10;
constexpr double THRESHOLD_DB = -35.0;
constexpr int    MAX_PEAKS_PER_FRAME = 15;   // ny: reducer output og DB-størrelse

// Fingerprints
constexpr int FAN_OUT = 10;
constexpr int MAX_DT  = 200;  // i frames
constexpr int MAX_ANCHORS_PER_FRAME = 6;     // ny: stabiliser fan-out

// Matching
constexpr int MIN_SCORE = 15;
