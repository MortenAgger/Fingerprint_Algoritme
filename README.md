# Fingerprint_Algoritme

A C++ prototype for an (audio) fingerprinting algorithm that combines **digital signal processing (DSB)** with **data structures & algorithms (DOA)** to identify/match a query against a small library of tracks.

> Repo structure: `src/`, `include/`, `Sange/`, and a `Makefile`. 

---

## What this project does

The program is intended as a learning project: build a “fingerprint” representation of audio files and use it to match/recognize audio by comparing a query against previously indexed tracks.

Typical building blocks in this type of project:
- Pre-processing of audio (sampling/normalization)
- Feature extraction in frequency domain (DSP)
- Building compact fingerprints (keys/hashes)
- Indexing fingerprints in efficient data structures (e.g., hash-based lookup)
- Matching/scoring and reporting best candidates

*(Exact implementation details depend on the code in `src/` and headers in `include/`.)*

---

## Project layout

- `src/` – C++ source files
- `include/` – header files
- `Sange/` – example audio files / dataset (“songs”)
- `Makefile` – build instructions/targets :contentReference[oaicite:2]{index=2}

---

## Build

### Requirements
- A C++ compiler (g++/clang++) with C++17 or newer
- `make`

### Compile
```bash
make
