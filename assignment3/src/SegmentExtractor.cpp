#include <iostream>
#include <filesystem>
#include <sndfile.hh>
#include <map>
#include "Constants.hpp"
#include "Helpers.hpp"

int main(int argc, char* argv[]) {
    std::string inputFilePath, outputFilePath;
    float noiseLevel = Constants::NOISE_LEVEL; // Default noise level
    int start = Constants::START_POINT; // Starting point in seconds
    int duration = Constants::SEGMENT_DUR; // Default duration in seconds

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

    // Check for required arguments: input file and output file
    if (args.find("-i") == args.end() && args.find("--input") == args.end()) {
        std::cerr << "Error: Missing required argument -i or --input for input file." << std::endl;
        std::cerr << "Usage: NoiseAdder -i <input_file> -o <output_file> -start <start_point> -dur <duration> -n <noise_level>" << std::endl;
        return EXIT_FAILURE;
    }
    if (args.find("-o") == args.end() && args.find("--output") == args.end()) {
        std::cerr << "Error: Missing required argument -o or --output for output file." << std::endl;
        std::cerr << "Usage: NoiseAdder -i <input_file> -o <output_file> -start <start_point> -dur <duration> -n <noise_level>" << std::endl;
        return EXIT_FAILURE;
    }

    // Extract values from the parsed arguments
    if (args.find("-i") != args.end()) {
        inputFilePath = args.at("-i");
    } else if (args.find("--input") != args.end()) {
        inputFilePath = args.at("--input");
    }

    if (args.find("-o") != args.end()) {
        outputFilePath = args.at("-o");
    } else if (args.find("--output") != args.end()) {
        outputFilePath = args.at("--output");
    }

    // Optional arguments: start point, duration, and noise level
    try {
        if (args.find("-start") != args.end()) {
            start = std::stoi(args.at("-start"));
        } else if (args.find("--start") != args.end()) {
            start = std::stoi(args.at("--start"));
        }
        if (args.find("-dur") != args.end()) {
            duration = std::stoi(args.at("-dur"));
        } else if (args.find("--duration") != args.end()) {
            duration = std::stoi(args.at("--duration"));
        }
        if (args.find("-n") != args.end()) {
            noiseLevel = std::stof(args.at("-n"));
        } else if (args.find("--noise") != args.end()) {
            noiseLevel = std::stof(args.at("--noise"));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Argument parsing error. " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // Validate input file
    if (!std::filesystem::exists(inputFilePath)) {
        std::cerr << "Error: Input file does not exist." << std::endl;
        return EXIT_FAILURE;
    }

    // Read the audio file
    SndfileHandle fileHandle(inputFilePath);
    if (fileHandle.error()) {
        std::cerr << "Error reading audio file: " << inputFilePath << std::endl;
        return EXIT_FAILURE;
    }

    int sampleRate = fileHandle.samplerate();
    int channels = fileHandle.channels();
    int totalFrames = static_cast<int>(fileHandle.frames());

    int startFrame = start * sampleRate;
    int segmentFrames = duration * sampleRate;

    if (startFrame + segmentFrames > totalFrames) {
        std::cerr << "Error: Specified segment is out of range." << std::endl;
        return EXIT_FAILURE;
    }

    // Buffer to store segment samples
    std::vector<short> buffer(segmentFrames * channels);
    fileHandle.seek(startFrame, SEEK_SET);
    fileHandle.readf(buffer.data(), segmentFrames);

    // Add noise to the segment if noise level is not zero
    if (noiseLevel > 0) {
        UTILS::addNoiseToAudio(buffer, noiseLevel);
    }

    // Write the segment to the output file
    SF_INFO sfinfo;
    sfinfo.frames = segmentFrames;
    sfinfo.samplerate = sampleRate;
    sfinfo.channels = channels;
    sfinfo.format = fileHandle.format();

    SndfileHandle outFile(outputFilePath.c_str(), SFM_WRITE, sfinfo.format, sfinfo.channels, sfinfo.samplerate);
    if (outFile.error()) {
        std::cerr << "Error writing segment file: " << outputFilePath << std::endl;
        return EXIT_FAILURE;
    }

    if (outFile.writef(buffer.data(), segmentFrames) != segmentFrames) {
        std::cerr << "Error writing audio data to segment file: " << outputFilePath << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Segment saved to: " << outputFilePath << std::endl;

    return EXIT_SUCCESS;
}
