#include <chrono>
#include "include/Helpers.hpp"

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
    int parseInt(const char *arg) {
        char* end;
        errno = 0;
        long val = std::strtol(arg, &end, 10);
        if (errno != 0 || *end != '\0' || val < 0) {
            std::cerr << "Error: Invalid argument " << arg << std::endl;
            exit(EXIT_FAILURE);
        }
        return static_cast<int>(val);
    }

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
    std::vector<unsigned char> computeFFTSignature(SndfileHandle &audioFile, int ws, int sh, int ds, int nf) {
        // Check if the audio file is valid
        if (audioFile.error()) {
            std::cerr << "Error: invalid audio file\n";
            exit(EXIT_FAILURE);
        }

        // Check if the sample rate is 44100 Hz
        if (audioFile.samplerate() != 44100) {
            std::cerr << "Error: currently supports only 44100 Hz of sample rate\n";
            exit(EXIT_FAILURE);
        }

        std::vector<unsigned char> binarySignature;

        // Get the number of frames and channels in the audio file
        int nFrames = static_cast<int>(audioFile.frames());
        int nChannels = audioFile.channels();

        // Read the audio samples
        std::vector<short> samples(nFrames * nChannels);
        audioFile.readf(samples.data(), nFrames);

        // Allocate memory for FFT input and output
        auto* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * ws);
        auto* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * ws);
        fftw_plan plan = fftw_plan_dft_1d(ws, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

        std::vector<double> power(ws / 2);
        std::vector<unsigned> maxPowerIdx(ws / 2);

        // Process the audio in overlapping windows
        for (int n = 0; n <= (nFrames - ws * ds) / (sh * ds); ++n) {
            // Prepare the FFT input by averaging over all channels and downsampling
            for (int k = 0; k < ws; ++k) {
                in[k][0] = 0.0;
                for (int ch = 0; ch < nChannels; ++ch) {
                    in[k][0] += static_cast<double>(samples[(n * (sh * ds) + k * ds) * nChannels + ch]);
                    for (int l = 1; l < ds; ++l) {
                        in[k][0] += static_cast<double>(samples[(n * (sh * ds) + k * ds + l) * nChannels + ch]);
                    }
                }
                // Average over all channels
                in[k][0] /= nChannels;
            }

            // Execute FFT
            fftw_execute(plan);

            // Compute power spectrum
            for (int k = 0; k < static_cast<int>(power.size()); ++k) {
                power[k] = out[k][0] * out[k][0] + out[k][1] * out[k][1];
            }

            // Find the indices of the most significant frequencies
            std::iota(maxPowerIdx.begin(), maxPowerIdx.end(), 0);
            std::partial_sort(maxPowerIdx.begin(), maxPowerIdx.begin() + nf, maxPowerIdx.end(), [&power](int i, int j) {
                return power[i] > power[j];
            });

            // Store the most significant frequencies as a binary signature
            for (int i = 0; i < nf; ++i) {
                auto c = maxPowerIdx[i] > 255 ? 255 : maxPowerIdx[i];
                binarySignature.push_back(static_cast<unsigned char>(c));
            }
        }

        // Clean up FFT resources
        fftw_destroy_plan(plan);
        fftw_free(in);
        fftw_free(out);

        return binarySignature;
    }

    /**
     * Function to add noise to an audio signal.
     *
     * This function adds Gaussian noise to each sample of the audio signal.
     *
     * @param samples A vector of audio samples to which noise will be added.
     * @param noiseLevel The standard deviation of the Gaussian noise.
     * @return A new vector of audio samples with added noise.
     */
    std::vector<short> addNoiseToAudio(const std::vector<short>& samples, float noiseLevel) {
        const int shortMin = static_cast<int>(std::numeric_limits<short>::min()), shortMax = static_cast<int>(std::numeric_limits<short>::max());
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

        std::vector<short> noisySamples = samples;
        std::default_random_engine generator(seed);
        std::normal_distribution<float> distribution(0.0, noiseLevel);

        for (auto &sample : noisySamples) {
            float noise = distribution(generator);
            int noisySample = static_cast<int>(sample) + static_cast<int>(noise);
            sample = static_cast<short>(std::clamp(noisySample, shortMin, shortMax));
        }

        return noisySamples;
    }

    /**
     * Function to compute the Normalized Compression Distance (NCD) between two signatures using compression.
     *
     * This function computes the NCD between two binary signatures using the specified
     * compression method.
     *
     * @param sig1 The first binary signature.
     * @param sig2 The second binary signature.
     * @param method The compression method to use.
     * @return The computed NCD value.
     */
    double computeNCD(const std::vector<unsigned char>& sig1, const std::vector<unsigned char>& sig2, COMPRESSOR::CompressionMethod method) {
        auto compressedSig1 = COMPRESSOR::compress(sig1, method);
        auto compressedSig2 = COMPRESSOR::compress(sig2, method);
        auto compressedConcat = COMPRESSOR::compress(sig1, sig2, method);

        size_t Cx = compressedSig1.size();
        size_t Cy = compressedSig2.size();
        size_t Cxy = compressedConcat.size();

        return (double)(Cxy - std::min(Cx, Cy)) / (double)std::max(Cx, Cy);
    }

    /**
     * Function to compute the Normalized Compression Distance (NCD) between two signatures without compression.
     *
     * This function computes the NCD between two binary signatures without using any compression.
     *
     * @param sig1 The first binary signature.
     * @param sig2 The second binary signature.
     * @return The computed NCD value.
     */
    double computeNCD(const std::vector<unsigned char>& sig1, const std::vector<unsigned char>& sig2) {
        size_t x = sig1.size();
        size_t y = sig2.size();
        size_t xy = sig1.size() + sig2.size();
        return static_cast<double>(xy - std::min(x, y)) / static_cast<double>(std::max(x, y));
    }

} // namespace UTILS
