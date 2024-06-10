#pragma once

namespace Constants {

    // Size of the FFT window for computing the FFT signature
    const int WINDOW_SIZE = 1024;

    // Amount of overlap between FFT windows
    const int SHIFT = 256;

    // Down-sampling factor for the audio signal
    const int DOWNSAMPLING = 4;

    // Number of significant frequencies to identify in each FFT window
    const int NUM_FREQS = 4;

    // Duration of a segment in seconds (Default)
    const int SEGMENT_DUR = 10;

    // Start point of a segment (Default)
    const int START_POINT = 10;

    // Default noise level to add to the audio signal
    const float NOISE_LEVEL = 0.05;

}
