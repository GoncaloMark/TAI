#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <sndfile.hh>
#include <fftw3.h>
#include <bitset>
#include <random>
#include "Compressor.hpp"

namespace UTILS {

    /**
     * Function to parse integer arguments safely.
     *
     * This function takes a string representation of an integer, converts it
     * to an integer, and checks for any errors during conversion.
     *
     * @param arg The string representation of the integer.
     * @return The parsed integer value.
     */
    int parseInt(const char* arg);

    /**
     * Function to compute the FFT signature of an audio file.
     *
     * This function processes a stereo audio file, converting it to mono and then
     * computing the FFT of overlapping windows of the audio signal. It identifies
     * the most significant frequencies in each window and stores their indices in
     * a binary format.
     *
     * @param audioFile A reference to the SndfileHandle object representing the audio file.
     * @param ws The size of the window for computing the FFT.
     * @param sh The window overlap.
     * @param ds The down-sampling factor.
     * @param nf The number of significant frequencies to identify.
     * @return A vector of unsigned char containing the binary signature of the audio file.
     */
    std::vector<unsigned char> computeFFTSignature(SndfileHandle &audioFile, int ws, int sh, int ds, int nf);

    /**
     * Function to add noise to an audio signal.
     *
     * This function adds Gaussian noise to each sample of the audio signal.
     *
     * @param samples A vector of audio samples to which noise will be added.
     * @param noiseLevel The standard deviation of the Gaussian noise.
     */
    void addNoiseToAudio(std::vector<short>& samples, float noiseLevel);

    /**
     * Function to compute the Normalized Compression Distance (NCD) between two signatures.
     *
     * This function computes the NCD between two binary signatures using the specified
     * compression method.
     *
     * @param sig1 The first binary signature.
     * @param sig2 The second binary signature.
     * @param method The compression method to use.
     * @return The computed NCD value.
     */
    double computeNCD(const std::vector<unsigned char>& sig1, const std::vector<unsigned char>& sig2, COMPRESSOR::CompressionMethod method);

    /**
     * Function to load a binary signature from a file.
     *
     * This function reads the contents of a file and returns it as a vector of unsigned char.
     *
     * @param filePath The path to the signature file.
     * @return A vector of unsigned char containing the binary signature.
     */
    std::vector<unsigned char> loadSignature(const std::string& filePath);

    /**
     * Function to add noise to an audio signal.
     *
     * This function adds Gaussian noise to each sample of the audio signal.
     *
     * @param samples A vector of audio samples to which noise will be added.
     * @param noiseLevel The standard deviation of the Gaussian noise.
     */
    void addNoiseToAudio(std::vector<short>& samples, float noiseLevel);

    /**
     * Function to save a binary signature to a file.
     *
     * This function writes the contents of a binary signature vector to a file.
     *
     * @param signature A vector of unsigned char containing the binary signature.
     * @param filePath The path to the file where the signature will be saved.
     */
    void saveSignatureToFile(const std::vector<unsigned char>& signature, const std::string& filePath);

} // namespace UTILS
