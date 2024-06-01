//
// Created by renan on 31/05/24.
//

#pragma once

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <algorithm>
#include <samplerate.h>
#include <numeric>
#include <vector>
#include <sndfile.hh>
#include <fftw3.h>
#include <sstream>
#include <bitset>
#include <sox.h>
#include <random>

namespace UTILS {

    // Function to parse integer arguments safely
    int parseInt(const char* arg);
    /**
    * Function to compute the FFT signature of an audio file.
    *
    * This function processes a stereo audio file, converting it to mono and then
    * computing the FFT of overlapping windows of the audio signal. It identifies
    * the most significant frequencies in each window and stores their indices in
    * a binary format.
    *
    * @param audioFile A reference to the Snd file Handle object representing the audio file.
    * @param ws The size of the window for computing the FFT (default is 1024).
    * @param sh The window overlap (default is 256).
    * @param ds The down-sampling factor (default is 4).
    * @param nf The number of significant frequencies to identify (default is 4).
    * @return A vector of unsigned char containing the binary signature of the audio file.
    */
    std::vector<unsigned char> computeFFTSignature(SndfileHandle &audioFile, int ws, int sh, int ds, int nf);
    void addNoiseToAudio(std::vector<short>& samples, float noiseLevel);
    void createSegments(const std::string& inputFilePath, const std::string& outputDir, int segmentDuration);

} // UTILS

