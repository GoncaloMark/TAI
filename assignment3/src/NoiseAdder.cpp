#include <iostream>
#include <filesystem>
#include <map>
#include <random>
#include <string>
#include <sndfile.hh>
#include "Constants.hpp"

int main(int argc, char* argv[]) {
    std::string inputFilePath;
    int segmentDuration = Constants::SEGMENT_DUR;

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
        std::cerr << "Error: Missing required argument -i or --input for input file." << std::endl;
        std::cerr << "Usage: SegmentExtractor -i <input_file> -d <segment_duration>" << std::endl;
        return EXIT_FAILURE;
    }

    if (args.find("-i") != args.end()) {
        inputFilePath = args.at("-i");
    } else if (args.find("--input") != args.end()) {
        inputFilePath = args.at("--input");
    }

    try {
        if (args.find("-d") != args.end()) {
            segmentDuration = std::stoi(args.at("-d"));
        } else if (args.find("--duration") != args.end()) {
            segmentDuration = std::stoi(args.at("--duration"));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Argument <segment_duration> must be an integer. " << e.what() << std::endl;
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

    if (fileHandle.samplerate() != Constants::SAMPLE_RATE) {
        std::cerr << "Error: Audio file does not have a sample rate of " << Constants::SAMPLE_RATE << " Hz." << std::endl;
        return EXIT_FAILURE;
    }

    int sampleRate = fileHandle.samplerate();
    int channels = fileHandle.channels();
    int totalFrames = fileHandle.frames();
    int framesPerSegment = segmentDuration * sampleRate;

    // Check if the audio file is long enough to extract a segment
    if (totalFrames < framesPerSegment) {
        std::cerr << "Error: Audio file is shorter than the requested segment duration." << std::endl;
        return EXIT_FAILURE;
    }

    // Generate a random starting point for the segment
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, totalFrames - framesPerSegment);
    int startFrame = dist(gen);

    // Read the segment from the random starting point
    std::vector<short> buffer(framesPerSegment * channels);
    fileHandle.seek(startFrame, SEEK_SET);
    int readCount = fileHandle.readf(buffer.data(), framesPerSegment);
    if (readCount <= 0) {
        std::cerr << "Error: Could not read frames from the input file." << std::endl;
        return EXIT_FAILURE;
    }

    // Generate output file path
    std::string outputFilePath = std::filesystem::path(inputFilePath).stem().string() + "_segment_" + std::to_string(startFrame) + ".wav";

    // Write the segment to the output file
    SF_INFO sfinfo;
    sfinfo.frames = readCount;
    sfinfo.samplerate = sampleRate;
    sfinfo.channels = channels;
    sfinfo.format = fileHandle.format();

    SndfileHandle outFile(outputFilePath.c_str(), SFM_WRITE, sfinfo.format, sfinfo.channels, sfinfo.samplerate);
    if (outFile.error()) {
        std::cerr << "Error writing segment file: " << outputFilePath << std::endl;
        return EXIT_FAILURE;
    }

    if (outFile.writef(buffer.data(), readCount) != readCount) {
        std::cerr << "Error writing audio data to segment file: " << outputFilePath << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Segment extracted and saved to: " << outputFilePath << std::endl;

    return EXIT_SUCCESS;
}
