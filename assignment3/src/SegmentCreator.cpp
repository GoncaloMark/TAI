#include <iostream>
#include <filesystem>
#include <map>
#include <string>
#include "Helpers.hpp"

bool validateArguments(const std::map<std::string, std::string>& args) {
    if (args.find("-i") == args.end() || args.find("--input") == args.end()) {
        std::cerr << "Error: Missing required argument -i or --input for input directory." << std::endl;
        return false;
    }
    if (args.find("-o") == args.end() || args.find("--output") == args.end()) {
        std::cerr << "Error: Missing required argument -o or --output for output directory." << std::endl;
        return false;
    }
    if (args.find("-d") == args.end() || args.find("--duration") == args.end()) {
        std::cerr << "Error: Missing required argument -d or --duration for segment duration." << std::endl;
        return false;
    }
    try {
        std::stoi(args.at("-d"));
    } catch (...) {
        std::cerr << "Error: Segment duration must be an integer." << std::endl;
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    std::map<std::string, std::string> args;
    for (int i = 1; i < argc; i += 2) {
        if (i + 1 < argc && argv[i][0] == '-') {
            args[argv[i]] = argv[i + 1];
        } else {
            std::cerr << "Error: Invalid argument format or missing value for " << argv[i] << std::endl;
            return 1;
        }
    }

    if (!validateArguments(args)) {
        std::cerr << "Usage: " << argv[0] << " -i <input_directory> -o <output_directory> -d <segment_duration>" << std::endl;
        return 1;
    }

    std::string inputDir = args["-i"];
    std::string outputDir = args["-o"];
    int segmentDuration = std::stoi(args["-d"]);

    for (const auto& entry : std::filesystem::directory_iterator(inputDir)) {
        if (entry.path().extension() == ".wav") {
            UTILS::createSegments(entry.path().string(), outputDir, segmentDuration);
        }
    }

    return 0;
}
