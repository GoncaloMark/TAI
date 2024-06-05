#include <iostream>
#include <filesystem>
#include <map>
#include <string>
#include <sndfile.hh>
#include "Helpers.hpp"
#include "Constants.hpp"

int main(int argc, char* argv[]) {
    std::string inputDir, outputDir;
    int ws = Constants::WINDOW_SIZE, sh = Constants::SHIFT, ds = Constants::DOWNSAMPLING, nf = Constants::NUM_FREQS;

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

    // Check for required arguments: input directory and output directory
    if (args.find("-i") == args.end() && args.find("--input") == args.end()) {
        std::cerr << "Error: Missing required argument -i or --input for input directory." << std::endl;
        std::cerr << "Usage: SignaturesCreator -i <input_directory> -o <output_directory> -ws <window_size> -sh <shift> -ds <downsampling> -nf <num_freqs>" << std::endl;
        return EXIT_FAILURE;
    }
    if (args.find("-o") == args.end() && args.find("--output") == args.end()) {
        std::cerr << "Error: Missing required argument -o or --output for output directory." << std::endl;
        std::cerr << "Usage: SignaturesCreator -i <input_directory> -o <output_directory> -ws <window_size> -sh <shift> -ds <downsampling> -nf <num_freqs>" << std::endl;
        return EXIT_FAILURE;
    }

    // Extract values from the parsed arguments
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

    try {
        if (args.find("-ws") != args.end()) {
            ws = std::stoi(args.at("-ws"));
        } else if (args.find("--window_size") != args.end()) {
            ws = std::stoi(args.at("--window_size"));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Argument <window_size> must be an integer. " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    try {
        if (args.find("-sh") != args.end()) {
            sh = std::stoi(args.at("-sh"));
        } else if (args.find("--shift") != args.end()) {
            sh = std::stoi(args.at("--shift"));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Argument <shift> must be an integer. " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    try {
        if (args.find("-ds") != args.end()) {
            ds = std::stoi(args.at("-ds"));
        } else if (args.find("--downsampling") != args.end()) {
            ds = std::stoi(args.at("--downsampling"));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Argument <downsampling> must be an integer. " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    try {
        if (args.find("-nf") != args.end()) {
            nf = std::stoi(args.at("-nf"));
        } else if (args.find("--num_freqs") != args.end()) {
            nf = std::stoi(args.at("--num_freqs"));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Argument <num_freqs> must be an integer. " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // Validate input and output directories
    if (!std::filesystem::exists(inputDir) || !std::filesystem::is_directory(inputDir)) {
        std::cerr << "Error: Input directory does not exist or is not a directory." << std::endl;
        return EXIT_FAILURE;
    }
    if (!std::filesystem::exists(outputDir)) {
        if (!std::filesystem::create_directories(outputDir)) {
            std::cerr << "Error: Could not create output directory." << std::endl;
            return EXIT_FAILURE;
        }
    }

    // Process each track's segment directory
    for (const auto& trackDirEntry : std::filesystem::directory_iterator(inputDir)) {
        if (trackDirEntry.is_directory()) {
            std::string trackName = trackDirEntry.path().stem().string();
            std::string trackOutputDir = outputDir + "/" + trackName;

            if (!std::filesystem::exists(trackOutputDir)) {
                if (!std::filesystem::create_directories(trackOutputDir)) {
                    std::cerr << "Error: Could not create directory for track " << trackOutputDir << std::endl;
                    continue;
                }
            }

            // Process each segment file in the track directory
            for (const auto& segmentEntry : std::filesystem::directory_iterator(trackDirEntry.path())) {
                if (segmentEntry.path().extension() == ".wav") {
                    SndfileHandle fileHandle(segmentEntry.path().string());
                    if (fileHandle.error()) {
                        std::cerr << "Error reading audio file: " << segmentEntry.path().string() << std::endl;
                        continue;
                    }

                    // Compute the FFT signature of the audio segment
                    auto signature = UTILS::computeFFTSignature(fileHandle, ws, sh, ds, nf);

                    // Construct the output file path for the signature
                    std::string outputFilePath = trackOutputDir + "/" + segmentEntry.path().stem().string() + ".sig";

                    // Write the signature to the output file
                    std::ofstream outputFile(outputFilePath, std::ios::binary);
                    if (!outputFile) {
                        std::cerr << "Error opening output file: " << outputFilePath << std::endl;
                        continue;
                    }

                    outputFile.write(reinterpret_cast<const char*>(signature.data()), static_cast<long>(signature.size()));
                    outputFile.close();
                }
            }
        }
    }

    return EXIT_SUCCESS;
}
