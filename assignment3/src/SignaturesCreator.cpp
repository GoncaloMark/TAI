#include <iostream>
#include <fstream>
#include <algorithm>
#include <sndfile.hh>
#include <bitset>
#include <filesystem>
#include <map>
#include "Helpers.hpp"
#include "Constants.hpp"

void helpMessage() {
    std::cerr << "Usage: MusicSigExtractor [options]\n"
              << "Options:\n"
              << "  -v, --verbose           Enable verbose mode\n"
              << "  -ws, --window-size      Set the window size (default: " << Constants::WINDOW_SIZE << ")\n"
              << "  -sh, --shift            Set the shift size (default: " << Constants::SHIFT << ")\n"
              << "  -ds, --downsampling     Set the downsampling rate (default: " << Constants::DOWNSAMPLING << ")\n"
              << "  -nf, --num-freqs        Set the number of frequencies (default: " << Constants::NUM_FREQS << ")\n"
              << "  -i, --input             Set the input directory\n"
              << "  -o, --output            Set the output directory\n"
              << "  -d, --duration          Set the segment duration (default: " << Constants::SEGMENT_DUR << " seconds)\n";
}

std::map<std::string, std::string> parseArguments(int argc, char* argv[]) {
    std::map<std::string, std::string> args;
    for (int i = 1; i < argc; i += 2) {
        if (i + 1 < argc && argv[i][0] == '-') {
            args[argv[i]] = argv[i + 1];
        } else {
            std::cerr << "Error: Invalid argument format or missing value for " << argv[i] << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    return args;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        helpMessage();
        return EXIT_FAILURE;
    }

    std::map<std::string, std::string> args = parseArguments(argc, argv);

    bool verbose = args.count("-v") || args.count("--verbose");
    int ws = args.count("-ws") ? std::stoi(args["-ws"]) : Constants::WINDOW_SIZE;
    ws = args.count("--window-size") ? std::stoi(args["--window-size"]) : ws;
    int sh = args.count("-sh") ? std::stoi(args["-sh"]) : Constants::SHIFT;
    sh = args.count("--shift") ? std::stoi(args["--shift"]) : sh;
    int ds = args.count("-ds") ? std::stoi(args["-ds"]) : Constants::DOWNSAMPLING;
    ds = args.count("--downsampling") ? std::stoi(args["--downsampling"]) : ds;
    int nf = args.count("-nf") ? std::stoi(args["-nf"]) : Constants::NUM_FREQS;
    nf = args.count("--num-freqs") ? std::stoi(args["--num-freqs"]) : nf;
    int segmentDuration = args.count("-d") ? std::stoi(args["-d"]) : Constants::SEGMENT_DUR;
    segmentDuration = args.count("--duration") ? std::stoi(args["--duration"]) : segmentDuration;

    std::string inputDir = args.count("-i") ? args["-i"] : args["--input"];
    std::string outputDir = args.count("-o") ? args["-o"] : args["--output"];

    if (verbose) {
        std::cout << "Input Directory: " << inputDir << "\n"
                  << "Output Directory: " << outputDir << "\n"
                  << "Window Size: " << ws << "\n"
                  << "Shift Size: " << sh << "\n"
                  << "Downsampling: " << ds << "\n"
                  << "Number of Frequencies: " << nf << "\n"
                  << "Segment Duration: " << segmentDuration << " seconds\n";
    }

    std::filesystem::create_directories(outputDir);

    for (const auto& entry : std::filesystem::directory_iterator(inputDir)) {
        if (entry.path().extension() == ".wav") {
            std::string inputFilePath = entry.path().string();
            SndfileHandle audioFile(inputFilePath);

            if (audioFile.error()) {
                std::cerr << "Error reading audio file: " << inputFilePath << std::endl;
                continue;
            }

            std::string trackOutputDir = outputDir + "/" + entry.path().stem().string();
            std::filesystem::create_directories(trackOutputDir);

            int sampleRate = audioFile.samplerate();
            int channels = audioFile.channels();
            int framesPerSegment = segmentDuration * sampleRate;

            std::vector<short> buffer(framesPerSegment * channels);
            int segmentNumber = 0;
            int readCount = 0;

            while ((readCount = audioFile.readf(buffer.data(), framesPerSegment)) > 0) {
                std::vector<short> segmentData(buffer.begin(), buffer.begin() + readCount * channels);
                auto signature = UTILS::computeFFTSegmentSignature(segmentData, sampleRate, channels, ws, sh, ds, nf);

                std::string outputFilePath = trackOutputDir + "/segment_" + std::to_string(segmentNumber++) + ".sig";
                std::ofstream os(outputFilePath, std::ofstream::binary);
                if (!os) {
                    std::cerr << "Warning: failed to open file to write: " << outputFilePath << std::endl;
                    continue;
                }
                os.write(reinterpret_cast<const char*>(signature.data()), static_cast<long>(signature.size()));
            }
        }
    }

    return EXIT_SUCCESS;
}
