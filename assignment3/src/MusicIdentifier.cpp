#include <iostream>
#include <filesystem>
#include <map>
#include <string>
#include <vector>
#include "Helpers.hpp"
#include "Compressor.hpp"

int main(int argc, char* argv[]) {
    std::string queryFilePath, databaseDir, compressionMethodStr;
    COMPRESSOR::CompressionMethod compressionMethod = COMPRESSOR::CompressionMethod::GZIP; // Default method

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

    if (args.find("-q") == args.end() && args.find("--query") == args.end()) {
        std::cerr << "Error: Missing required argument -q or --query for query file." << std::endl;
        std::cerr << "Usage: MusicIdentifier -q <query_file> -d <database_dir> -m <compression_method>" << std::endl;
        return EXIT_FAILURE;
    }
    if (args.find("-d") == args.end() && args.find("--database") == args.end()) {
        std::cerr << "Error: Missing required argument -d or --database for database directory." << std::endl;
        std::cerr << "Usage: MusicIdentifier -q <query_file> -d <database_dir> -m <compression_method>" << std::endl;
        return EXIT_FAILURE;
    }
    if (args.find("-m") == args.end() && args.find("--method") == args.end()) {
        std::cerr << "Error: Missing required argument -m or --method for compression method." << std::endl;
        std::cerr << "Usage: MusicIdentifier -q <query_file> -d <database_dir> -m <compression_method>" << std::endl;
        return EXIT_FAILURE;
    }

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
    } else if (args.find("--method") != args.end()) {
        compressionMethodStr = args.at("--method");
    }

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

    // Validate query file and database directory
    if (!std::filesystem::exists(queryFilePath)) {
        std::cerr << "Error: Query file does not exist." << std::endl;
        return EXIT_FAILURE;
    }
    if (!std::filesystem::exists(databaseDir) || !std::filesystem::is_directory(databaseDir)) {
        std::cerr << "Error: Database directory does not exist or is not a directory." << std::endl;
        return EXIT_FAILURE;
    }

    try {
        // Load query signature
        auto querySignature = UTILS::loadSignature(queryFilePath);

        // Iterate through database directory and compute NCD
        double minNCD = std::numeric_limits<double>::max();
        std::string bestMatchTrack;

        for (const auto& trackDir : std::filesystem::directory_iterator(databaseDir)) {
            if (trackDir.is_directory()) {
                std::string trackName = trackDir.path().filename().string();
                for (const auto& segmentFile : std::filesystem::directory_iterator(trackDir.path())) {
                    if (segmentFile.path().extension() == ".sig") {
                        auto trackSignature = UTILS::loadSignature(segmentFile.path().string());
                        double ncd = UTILS::computeNCD(querySignature, trackSignature, compressionMethod);
                        std::cout << "NCD with " << segmentFile.path().filename() << " in track " << trackName << ": " << ncd << std::endl;

                        if (ncd < minNCD) {
                            minNCD = ncd;
                            bestMatchTrack = trackName;
                        }
                    }
                }
            }
        }

        std::cout << "Best match track: " << bestMatchTrack << " with NCD: " << minNCD << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
