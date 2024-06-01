#include <iostream>
#include <filesystem>
#include <map>
#include <string>
#include "Helpers.hpp"
#include "Constants.hpp"

int main(int argc, char* argv[]) {
    std::string inputDir = "", outputDir = "";
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
        std::cerr << "Error: Missing required argument -i or --input for input directory." << std::endl;
        std::cerr << "Usage: SegmentCreator -i <input_directory> -o <output_directory> -d <segment_duration>" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (args.find("-o") == args.end() && args.find("--output") == args.end()) {
        std::cerr << "Error: Missing required argument -o or --output for output directory." << std::endl;
        std::cerr << "Usage: SegmentCreator -i <input_directory> -o <output_directory> -d <segment_duration>" << std::endl;
        exit(EXIT_FAILURE);
    }
    try {
        if (args.find("-d") != args.end()) {
            std::stoi(args.at("-d"));
        } else if(args.find("--duration") != args.end()) {
            std::stoi(args.at("--duration"));
        }
    } catch (...) {
        std::cerr << "Error: Segment duration must be an integer." << std::endl;
        std::cerr << "Usage: SegmentCreator -i <input_directory> -o <output_directory> -d <segment_duration>" << std::endl;
        exit(EXIT_FAILURE);
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
    if (args.find("-d") != args.end()) {
        segmentDuration = UTILS::parseInt(args.at("-d").c_str());
    } else if (args.find("--duration") != args.end()) {
        segmentDuration = UTILS::parseInt(args.at("--duration").c_str());
    }

    // Start program
    for (const auto& entry : std::filesystem::directory_iterator(inputDir)) {
        if (entry.path().extension() == ".wav") {
            UTILS::createSegments(entry.path().string(), outputDir, segmentDuration);
        }
    }

    return EXIT_SUCCESS;
}
