#include "include/Helpers.hpp"

namespace UTILS {
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

    std::vector<unsigned char> computeFFTSignature(SndfileHandle &audioFile, int ws, int sh, int ds, int nf) {
        if (audioFile.error()) {
            std::cerr << "Error: invalid audio file\n";
            exit(EXIT_FAILURE);
        }
        if (audioFile.samplerate() != 44100) {
            std::cerr << "Error: currently supports only 44100 Hz of sample rate\n";
            exit(EXIT_FAILURE);
        }

        std::vector<unsigned char> binarySignature;
        int nFrames = static_cast<int>(audioFile.frames());
        int nChannels = audioFile.channels();
        std::vector<short> samples(nFrames * nChannels);
        audioFile.readf(samples.data(), nFrames);

        auto* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * ws);
        auto* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * ws);
        fftw_plan plan = fftw_plan_dft_1d(ws, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

        std::vector<double> power(ws / 2);
        std::vector<unsigned> maxPowerIdx(ws / 2);

        for (int n = 0; n <= (nFrames - ws * ds) / (sh * ds); ++n) {
            for (int k = 0; k < ws; ++k) {
                in[k][0] = 0.0;
                for (int ch = 0; ch < nChannels; ++ch) {
                    in[k][0] += static_cast<double>(samples[(n * (sh * ds) + k * ds) * nChannels + ch]);
                    for (int l = 1; l < ds; ++l) {
                        in[k][0] += static_cast<double>(samples[(n * (sh * ds) + k * ds + l) * nChannels + ch]);
                    }
                }
                in[k][0] /= nChannels; // Average over all channels
            }

            fftw_execute(plan);

            for (int k = 0; k < static_cast<int>(power.size()); ++k) {
                power[k] = out[k][0] * out[k][0] + out[k][1] * out[k][1];
            }

            std::iota(maxPowerIdx.begin(), maxPowerIdx.end(), 0);
            std::partial_sort(maxPowerIdx.begin(), maxPowerIdx.begin() + nf, maxPowerIdx.end(), [&power](int i, int j) {
                return power[i] > power[j];
            });

            for (int i = 0; i < nf; ++i) {
                auto c = maxPowerIdx[i] > 255 ? 255 : maxPowerIdx[i];
                binarySignature.push_back(static_cast<unsigned char>(c));
            }
        }

        fftw_destroy_plan(plan);
        fftw_free(in);
        fftw_free(out);

        return binarySignature;
    }
    void addNoiseToAudio(std::vector<short>& samples, float noiseLevel) {
        std::default_random_engine generator;
        std::normal_distribution<float> distribution(0.0, noiseLevel);

        for (auto &sample : samples) {
            sample += static_cast<short>(distribution(generator));
        }
    }

    void createSegments(const std::string& inputFilePath, const std::string& outputDir, int segmentDuration) {
        SndfileHandle fileHandle(inputFilePath);
        if (fileHandle.error()) {
            std::cerr << "Error reading audio file: " << inputFilePath << std::endl;
            return;
        }

        int sampleRate = fileHandle.samplerate();
        int channels = fileHandle.channels();
        int framesPerSegment = segmentDuration * sampleRate;

        std::vector<short> buffer(framesPerSegment * channels);
        int segmentNumber = 0;
        int readCount = 0;

        while ((readCount = fileHandle.readf(buffer.data(), framesPerSegment)) > 0) {
            std::string outputFilePath = outputDir + "/" + std::filesystem::path(inputFilePath).stem().string() +
                                         "_segment_" + std::to_string(segmentNumber++) + ".wav";

            SF_INFO sfinfo;
            sfinfo.frames = readCount;
            sfinfo.samplerate = sampleRate;
            sfinfo.channels = channels;
            sfinfo.format = fileHandle.format();

            SndfileHandle outFile(outputFilePath.c_str(), SFM_WRITE, sfinfo.format, sfinfo.channels, sfinfo.samplerate);
            if (outFile.error()) {
                std::cerr << "Error writing segment file: " << outputFilePath << std::endl;
                std::cerr << "Error details: " << sf_strerror(NULL) << std::endl;  // Print the specific libsndfile error message
                continue;
            }

            if (outFile.writef(buffer.data(), readCount) != readCount) {
                std::cerr << "Error writing audio data to segment file: " << outputFilePath << std::endl;
            }
        }
    }
} // UTILS