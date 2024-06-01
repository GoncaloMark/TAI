#include <iostream>
#include <filesystem>
#include <map>
#include <string>
#include <sndfile.hh>
#include "Helpers.hpp"
#include "Constants.hpp"

int main(int argc, char* argv[]) {
    std::string inputDir, outputDir;
    int segmentDuration = Constants::SEGMENT_DUR;
    float noiseLevel = Constants::NOISE_LEVEL;

    // Parse and Validate Arguments
    std::map<std::string, std::string> args;
    for (int i = 1; i < argc; i += 2) {
        if (i + 1 < argc && argv[i][0] == '-') {
            args[argv[i]] = argv[i + 1];
        } else {
            std::cerr << "Error: Invalid argument format or missing value for " << argv[i] << std::endl;
            return EXIT_FAILURE;
        }
    }

    if (args.find("-i") == args.end() && args.find("--input") == args.end()) {
        std::cerr << "Error: Missing required argument -i or --input for input directory." << std::endl;
        std::cerr << "Usage: SegmentsCreator -i <input_directory> -o <output_directory> -d <segment_duration> -n <noise_level>" << std::endl;
        return EXIT_FAILURE;
    }
    if (args.find("-o") == args.end() && args.find("--output") == args.end()) {
        std::cerr << "Error: Missing required argument -o or --output for output directory." << std::endl;
        std::cerr << "Usage: SegmentsCreator -i <input_directory> -o <output_directory> -d <segment_duration> -n <noise_level>" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        if (args.find("-d") != args.end()) {
            segmentDuration = std::stoi(args.at("-d"));
        } else if (args.find("--duration") != args.end()) {
            segmentDuration = std::stoi(args.at("--duration"));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Segment duration must be an integer. " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    try {
        if (args.find("-n") != args.end()) {
            noiseLevel = std::stof(args.at("-n"));
        } else if (args.find("--noise") != args.end()) {
            noiseLevel = std::stof(args.at("--noise"));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Noise level must be a float. " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    if (args.find("-i") != args.end()) {
        inputDir = args.at("-i");
    } else if (args.find("--input") != args.end()) {
        inputDir = args.at("--input");
    }
    if (args.find("-o") != args.end()) {
        outputDir = args.at("-o");
    } else if (args.find("--output") != args.end()) {
        outputDir = args.at("--output");
    }

    // Validate input and output directories
    if (!std::filesystem::exists(inputDir) || !std::filesystem::is_directory(inputDir)) {
        std::cerr << "Error: Input directory does not exist or is not a directory." << std::endl;
        return EXIT_FAILURE;
    }
    if (!std::filesystem::exists(outputDir) && !std::filesystem::create_directories(outputDir)) {
        std::cerr << "Error: Could not create output directory." << std::endl;
        return EXIT_FAILURE;
    }

    // Start processing audio files
    for (const auto& entry : std::filesystem::directory_iterator(inputDir)) {
        if (entry.path().extension() == ".wav") {
            SndfileHandle fileHandle(entry.path().string());
            if (fileHandle.error()) {
                std::cerr << "Error reading audio file: " << entry.path().string() << std::endl;
                continue;
            }

            if (fileHandle.samplerate() != 44100) {
                std::cerr << "Error: Audio file " << entry.path().string() << " does not have a sample rate of 44100 Hz." << std::endl;
                continue;
            }

            std::string trackOutputDir = outputDir + "/" + entry.path().stem().string();
            if (!std::filesystem::exists(trackOutputDir) && !std::filesystem::create_directories(trackOutputDir)) {
                std::cerr << "Error: Could not create directory for track " << trackOutputDir << std::endl;
                continue;
            }

            int sampleRate = fileHandle.samplerate();
            int channels = fileHandle.channels();
            int framesPerSegment = segmentDuration * sampleRate;

            std::vector<short> buffer(framesPerSegment * channels);
            int segmentNumber = 0;
            int readCount = 0;

            while ((readCount = fileHandle.readf(buffer.data(), framesPerSegment)) > 0) {
                // Add noise to the segment
                UTILS::addNoiseToAudio(buffer, noiseLevel);

                std::string outputFilePath = trackOutputDir + "/" + std::to_string(segmentNumber++) + ".wav";

                SF_INFO sfinfo;
                sfinfo.frames = readCount;
                sfinfo.samplerate = sampleRate;
                sfinfo.channels = channels;
                sfinfo.format = fileHandle.format();

                SndfileHandle outFile(outputFilePath.c_str(), SFM_WRITE, sfinfo.format, sfinfo.channels, sfinfo.samplerate);
                if (outFile.error()) {
                    std::cerr << "Error writing segment file: " << outputFilePath << std::endl;
                    continue;
                }

                if (outFile.writef(buffer.data(), readCount) != readCount) {
                    std::cerr << "Error writing audio data to segment file: " << outputFilePath << std::endl;
                }
            }
        }
    }

    return EXIT_SUCCESS;
}
