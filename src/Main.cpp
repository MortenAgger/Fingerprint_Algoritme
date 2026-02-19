#include "ReadMusicFile.hpp"
#include "Spectrogram.hpp"
#include "Peakfinder.hpp"
#include "Fingerprint.hpp"
#include "Config.hpp"
#include "FingerprintDB.hpp"

#include <iostream>


std::vector<Fingerprint> processSong(const char* path)
{
    ReadMusicFile loader;
    auto samples = loader.loadMonoWav(path);

    Spectrogram spec;
    auto spectrogram = spec.computeSpectrogram(samples, WINDOW_SIZE, HOP_SIZE);

    PeakFinder finder;
    auto peaks = finder.findPeaks(spectrogram, NEIGHBORHOOD, THRESHOLD_DB);

    std::cout << "Processing: " << path << "\n";
    std::cout << "  Peaks: " << peaks.size() << "\n";

    Fingerprinter fp;
    auto fps = fp.generateFingerprints(peaks);

    std::cout << "  Fingerprints: " << fps.size() << "\n\n";

    return fps;
}


int main() {

        const char* CaliforniaGirls = "/Users/morten/Desktop/Fingerprint_Algoritme/Sange/California_girls.wav";
        const char* CaliforniaGirls10 = "/Users/morten/Desktop/Fingerprint_Algoritme/Sange/California_clip1.wav";
        const char* Espresso = "/Users/morten/Desktop/Fingerprint_Algoritme/Sange/Sabrina_Carpenter_Espresso.wav";

        auto fingerprintsCaliforniaGirls = processSong(CaliforniaGirls); 
        auto fingerprintsCaliforniaGirls10 = processSong(CaliforniaGirls10); 
        auto fingerprintsEspresso = processSong(Espresso); 

        FingerprintDB db;
        addSongToDatabase(db, fingerprintsCaliforniaGirls, 1);

        int result = identifySong(db, fingerprintsCaliforniaGirls10);
        std::cout <<  (result == 1 ? "Found California girls" : "False did not findCalifornia girls") << '\n';

        int resultv2 = identifySong(db, fingerprintsEspresso);
        std::cout <<  (resultv2 == 1 ? "Found Espresso" : "False did not Espresso") << '\n';

        int resultv3 = identifySong(db, fingerprintsCaliforniaGirls);
        std::cout <<  (resultv3 == 1 ? "Found California girls" : "False did not findCalifornia girls") << '\n';

  
        return 0;
    
}

