#include <iostream>
#include <filesystem>
#include <map>
#include <string>
#include <vector>
#include "Helpers.hpp"
#include "Compressor.hpp"
#include "Constants.hpp"

std::string HelpMessage() {
    return "Usage: MusicIdentifier -q <query_file> -d <database_dir> [-m <compression_method>] -ws <window_size> -sh <shift> -ds <downsampling> -nf <num_freqs>";
}

int main(int argc, char* argv[]) {
    std::string queryFilePath, databaseDir, compressionMethodStr;
    COMPRESSOR::CompressionMethod compressionMethod = COMPRESSOR::CompressionMethod::GZIP; // Default method
    bool useCompression = false;
    int windowSize = Constants::WINDOW_SIZE;
    int shift = Constants::SHIFT;
    int downSampling = Constants::DOWNSAMPLING;
    int numFreqs = Constants::NUM_FREQS;

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

    // Check for the required arguments: query file and database directory
    if (args.find("-q") == args.end() && args.find("--query") == args.end()) {
        std::cerr << "Error: Missing required argument -q or --query for query file." << std::endl;
        std::cerr << HelpMessage() << std::endl;
        return EXIT_FAILURE;
    }
    if (args.find("-d") == args.end() && args.find("--database") == args.end()) {
        std::cerr << "Error: Missing required argument -d or --database for database directory." << std::endl;
        std::cerr << HelpMessage() << std::endl;
        return EXIT_FAILURE;
    }

    // Extract values from the parsed arguments
    if (args.find("-q") != args.end()) {
        queryFilePath = args.at("-q");
    } else if (args.find("--query") != args.end()) {
        queryFilePath = args.at("--query");
    }

    if (args.find("-d") != args.end()) {
        databaseDir = args.at("-d");
    } else if (args.find("--database") != args.end()) {
        databaseDir = args.at("--database");
    }

    if (args.find("-m") != args.end()) {
        compressionMethodStr = args.at("-m");
        useCompression = true;
    } else if (args.find("--method") != args.end()) {
        compressionMethodStr = args.at("--method");
        useCompression = true;
    }

    // Optional arguments: window size, shift, downsampling, and number of frequencies
    try {
        if (args.find("-ws") != args.end()) {
            windowSize = std::stoi(args.at("-ws"));
        } else if (args.find("--window_size") != args.end()) {
            windowSize = std::stoi(args.at("--window_size"));
        }
        if (args.find("-sh") != args.end()) {
            shift = std::stoi(args.at("-sh"));
        } else if (args.find("--shift") != args.end()) {
            shift = std::stoi(args.at("--shift"));
        }
        if (args.find("-ds") != args.end()) {
            downSampling = std::stoi(args.at("-ds"));
        } else if (args.find("--downsampling") != args.end()) {
            downSampling = std::stoi(args.at("--downsampling"));
        }
        if (args.find("-nf") != args.end()) {
            numFreqs = std::stoi(args.at("-nf"));
        } else if (args.find("--num_freqs") != args.end()) {
            numFreqs = std::stoi(args.at("--num_freqs"));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Argument parsing error. " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // Determine the compression method if provided
    if (useCompression) {
        if (compressionMethodStr == "gzip") {
            compressionMethod = COMPRESSOR::CompressionMethod::GZIP;
        } else if (compressionMethodStr == "bzip2") {
            compressionMethod = COMPRESSOR::CompressionMethod::BZIP2;
        } else if (compressionMethodStr == "lzma") {
            compressionMethod = COMPRESSOR::CompressionMethod::LZMA;
        } else if (compressionMethodStr == "zstd") {
            compressionMethod = COMPRESSOR::CompressionMethod::ZSTD;
        } else {
            std::cerr << "Error: Unsupported compression method. Supported methods are: gzip, bzip2, lzma, zstd." << std::endl;
            return EXIT_FAILURE;
        }
    }

    // Validate the existence of the query file and the database directory
    if (!std::filesystem::exists(queryFilePath)) {
        std::cerr << "Error: Query file does not exist." << std::endl;
        return EXIT_FAILURE;
    }
    if (!std::filesystem::exists(databaseDir) || !std::filesystem::is_directory(databaseDir)) {
        std::cerr << "Error: Database directory does not exist or is not a directory." << std::endl;
        return EXIT_FAILURE;
    }

    try {
        // Load query audio file
        SndfileHandle queryFileHandle(queryFilePath);
        if (queryFileHandle.error()) {
            std::cerr << "Error reading query audio file: " << queryFilePath << std::endl;
            return EXIT_FAILURE;
        }

        // Compute the FFT signature of the query audio file
        auto querySignature = UTILS::computeFFTSignature(queryFileHandle, windowSize, shift, downSampling, numFreqs);

        // Initialize variables to find the best match
        double minNCD = std::numeric_limits<double>::max();
        std::string bestMatchTrack;

        // Iterate through the audio files in the database directory
        for (const auto& audioFile : std::filesystem::directory_iterator(databaseDir)) {
            if (audioFile.path().extension() == ".wav") {
                // Load the audio file
                SndfileHandle trackFileHandle(audioFile.path().string());
                if (trackFileHandle.error()) {
                    std::cerr << "Error reading audio file: " << audioFile.path().string() << std::endl;
                    continue;
                }

                // Compute the FFT signature of the current track
                auto trackSignature = UTILS::computeFFTSignature(trackFileHandle, windowSize, shift, downSampling, numFreqs);

                // Compute the Normalized Compression Distance (NCD) between the query and track signatures
                double ncd = useCompression ? UTILS::computeNCD(querySignature, trackSignature, compressionMethod)
                                            : UTILS::computeNCD(querySignature, trackSignature);

                std::cout << "NCD with " << audioFile.path().filename().string() << ": " << ncd << std::endl;

                // Update the best match if the current NCD is smaller
                if (ncd < minNCD) {
                    minNCD = ncd;
                    bestMatchTrack = audioFile.path().filename().string(); // Use the file name as the track name
                }
            }
        }

        // Print the best match track and its NCD
        std::cout << "Best match track: " << bestMatchTrack << " with NCD: " << minNCD << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
