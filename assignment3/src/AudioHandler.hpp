#pragma once

#include <iostream>
#include <sndfile.h>

class AudioHandler {

    static bool readAudioFile(const char* filename, float*& data, SF_INFO& info) {
        // Open the audio file
        SNDFILE *file = sf_open(filename, SFM_READ, &info);
        if (!file) {
            std::cerr << "Error opening file!" << std::endl;
            return false;
        }

        // Allocate buffer for audio data
        data = new float[info.frames * info.channels];

        // Read audio data
        sf_count_t count = sf_readf_float(file, data, info.frames);

        // Close the file
        sf_close(file);

        return true;
    }

};
