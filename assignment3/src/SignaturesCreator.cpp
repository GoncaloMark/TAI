#include <iostream>
#include <filesystem>
#include <map>
#include <string>
#include "Helpers.hpp"
#include "Constants.hpp"
#include <sndfile.hh>

int main(int argc, char* argv[]) {
    std::string inputDir = "", outputDir = "";
    int segmentDuration = Constants::SEGMENT_DUR;
    int ws = Constants::WINDOW_SIZE;
    int sh = Constants::SHIFT;
    int ds = Constants::DOWNSAMPLING;
    int nf = Constants::NUM_FREQS;

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
        std::cerr << "Usage: SegmentCreator -i <input_directory> -o <output_directory> -d <segment_duration> [-ws <window_size>] [-sh <shift>] [-ds <downsampling>] [-nf <num_freqs>]" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (args.find("-o") == args.end() && args.find("--output") == args.end()) {
        std::cerr << "Error: Missing required argument -o or --output for output directory." << std::endl;
        std::cerr << "Usage: SegmentCreator -i <input_directory> -o <output_directory> -d <segment_duration> [-ws <window_size>] [-sh <shift>] [-ds <downsampling>] [-nf <num_freqs>]" << std::endl;
        exit(EXIT_FAILURE);
    }
    try {
        if (args.find("-d") != args.end()) {
            segmentDuration = std::stoi(args.at("-d"));
        } else if(args.find("--duration") != args.end()) {
            segmentDuration = std::stoi(args.at("--duration"));
        }
        if (args.find("-ws") != args.end()) {
            ws = std::stoi(args.at("-ws"));
        } else if(args.find("--window_size") != args.end()) {
            ws = std::stoi(args.at("--window_size"));
        }
        if (args.find("-sh") != args.end()) {
            sh = std::stoi(args.at("-sh"));
        } else if(args.find("--shift") != args.end()) {
            sh = std::stoi(args.at("--shift"));
        }
        if (args.find("-ds") != args.end()) {
            ds = std::stoi(args.at("-ds"));
        } else if(args.find("--downsampling") != args.end()) {
            ds = std::stoi(args.at("--downsampling"));
        }
        if (args.find("-nf") != args.end()) {
            nf = std::stoi(args.at("-nf"));
        } else if(args.find("--num_freqs") != args.end()) {
            nf = std::stoi(args.at("--num_freqs"));
        }
    } catch (...) {
        std::cerr << "Error: Argument values must be integers." << std::endl;
        std::cerr << "Usage: SegmentCreator -i <input_directory> -o <output_directory> -d <segment_duration> [-ws <window_size>] [-sh <shift>] [-ds <downsampling>] [-nf <num_freqs>]" << std::endl;
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

    // Ensure output directory exists
    std::filesystem::create_directories(outputDir);

    // Process each WAV file in the input folder
    for (const auto& entry : std::filesystem::directory_iterator(inputDir)) {
        if (entry.path().extension() == ".wav") {
            std::string inputFilePath = entry.path().string();

            SndfileHandle fileHandle(inputFilePath);
            if (fileHandle.error()) {
                std::cerr << "Error reading audio file: " << inputFilePath << std::endl;
                continue;
            }

            int sampleRate = fileHandle.samplerate();
            int channels = fileHandle.channels();
            int framesPerSegment = segmentDuration * sampleRate;

            std::vector<short> buffer(framesPerSegment * channels);
            int segmentNumber = 0;
            int readCount = 0;

            while ((readCount = fileHandle.readf(buffer.data(), framesPerSegment)) > 0) {
                auto signature = UTILS::computeFFTSignature(fileHandle, ws, sh, ds, nf);

                std::string segmentDir = outputDir + "/" + std::filesystem::path(inputFilePath).stem().string();
                std::filesystem::create_directories(segmentDir);

                std::string outputFilePath = segmentDir + "/segment_" + std::to_string(segmentNumber++) + ".sig";

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
