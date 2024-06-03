#pragma once

namespace Constants {

    // Size of the FFT window for computing the FFT signature
    const int WINDOW_SIZE = 1024;

    // Amount of overlap between FFT windows
    const int SHIFT = 256;

    // Downsampling factor for the audio signal
    const int DOWNSAMPLING = 4;

    // Number of significant frequencies to identify in each FFT window
    const int NUM_FREQS = 4;

    // Duration of each segment in seconds
    const int SEGMENT_DUR = 10;

    // Sample rate of the audio files (in Hz)
    const int SAMPLE_RATE = 44100;

    // Default noise level to add to the audio signal
    const float NOISE_LEVEL = 0.05;

}
