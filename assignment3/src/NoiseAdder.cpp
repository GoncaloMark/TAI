#include <iostream>
#include <filesystem>
#include <sndfile.hh>
#include <map>
#include "Constants.hpp"
#include "Helpers.hpp"

int main(int argc, char* argv[]) {
    std::string inputFilePath, outputFilePath, signatureFilePath;
    float noiseLevel = Constants::NOISE_LEVEL; // Default noise level
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

    // Check for required arguments: input file, output file, and signature file
    if (args.find("-i") == args.end() && args.find("--input") == args.end()) {
        std::cerr << "Error: Missing required argument -i or --input for input file." << std::endl;
        std::cerr << "Usage: NoiseAdder -i <input_file> -o <output_file> -s <signature_file> -n <noise_level> -w <window_size> -sh <shift> -ds <downsampling> -nf <num_freqs>" << std::endl;
        return EXIT_FAILURE;
    }
    if (args.find("-o") == args.end() && args.find("--output") == args.end()) {
        std::cerr << "Error: Missing required argument -o or --output for output file." << std::endl;
        std::cerr << "Usage: NoiseAdder -i <input_file> -o <output_file> -s <signature_file> -n <noise_level> -w <window_size> -sh <shift> -ds <downsampling> -nf <num_freqs>" << std::endl;
        return EXIT_FAILURE;
    }
    if (args.find("-s") == args.end() && args.find("--signature") == args.end()) {
        std::cerr << "Error: Missing required argument -s or --signature for signature file." << std::endl;
        std::cerr << "Usage: NoiseAdder -i <input_file> -o <output_file> -s <signature_file> -n <noise_level> -w <window_size> -sh <shift> -ds <downsampling> -nf <num_freqs>" << std::endl;
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

    if (args.find("-s") != args.end()) {
        signatureFilePath = args.at("-s");
    } else if (args.find("--signature") != args.end()) {
        signatureFilePath = args.at("--signature");
    }

    // Optional arguments: noise level, window size, shift, downsampling, and number of frequencies
    try {
        if (args.find("-n") != args.end()) {
            noiseLevel = std::stof(args.at("-n"));
        } else if (args.find("--noise") != args.end()) {
            noiseLevel = std::stof(args.at("--noise"));
        }
        if (args.find("-w") != args.end()) {
            windowSize = std::stoi(args.at("-w"));
        } else if (args.find("--window") != args.end()) {
            windowSize = std::stoi(args.at("--window"));
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
    int frames = fileHandle.frames();

    // Buffer to store audio samples
    std::vector<short> buffer(frames * channels);
    fileHandle.readf(buffer.data(), frames);

    // Add noise to the audio samples
    UTILS::addNoiseToAudio(buffer, noiseLevel);

    // Write the noisy segment to the output file
    SF_INFO sfinfo;
    sfinfo.frames = frames;
    sfinfo.samplerate = sampleRate;
    sfinfo.channels = channels;
    sfinfo.format = fileHandle.format();

    SndfileHandle outFile(outputFilePath.c_str(), SFM_WRITE, sfinfo.format, sfinfo.channels, sfinfo.samplerate);
    if (outFile.error()) {
        std::cerr << "Error writing segment file: " << outputFilePath << std::endl;
        return EXIT_FAILURE;
    }

    if (outFile.writef(buffer.data(), frames) != frames) {
        std::cerr << "Error writing audio data to segment file: " << outputFilePath << std::endl;
        return EXIT_FAILURE;
    }

    // Create signature for the noisy segment
    SndfileHandle noisyFileHandle(outputFilePath);
    auto signature = UTILS::computeFFTSignature(noisyFileHandle, windowSize, shift, downSampling, numFreqs);

    // Save the signature to a file
    UTILS::saveSignatureToFile(signature, signatureFilePath);

    std::cout << "Noisy segment and signature saved to: " << outputFilePath << " and " << signatureFilePath << std::endl;

    return EXIT_SUCCESS;
}
