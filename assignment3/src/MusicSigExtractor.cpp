#include <iostream>
#include <fstream>
#include <algorithm>
#include <sndfile.hh>
#include <bitset>
#include <filesystem>
#include "Helpers.hpp"
#include "Constants.hpp"

using namespace std;

// Function to print help message
void helpMessage() {
    cerr << "Usage: GetMaxFreqs [ -v (verbose) ]" << endl;
    cerr << "                   [ -ws winSize ]" << endl;
    cerr << "                   [ -sh shift ]" << endl;
    cerr << "                   [ -ds downSampling ]" << endl;
    cerr << "                   [ -nf nFreqs ]" << endl;
    cerr << "                   [ -inDir AudioFilesDir ]" << endl;
    cerr << "                   [ -outDir freqsDir ]" << endl;
}

int main (int argc, char* argv[]) {

    // Declare variables
    bool verbose = false;
    int ws = Constants::WINDOW_SIZE;
    int sh = Constants::SHIFT;
    int ds = Constants::DOWNSAMPLING;
    int nf = Constants::NUM_FREQS;
    string inputFolder;
    string outputFolder;

    if(argc < 2) {
        helpMessage();
        return EXIT_FAILURE;
    }
    // Parse arguments
    for (int n = 1; n < argc; n++) {
        if (string(argv[n]) == "-v") {
            verbose = true;
        } else if (string(argv[n]) == "-inDir" && n + 1 < argc) {
            inputFolder = argv[n + 1];
        } else if (string(argv[n]) == "-outDir" && n + 1 < argc) {
            outputFolder = argv[n + 1];
        } else if (string(argv[n]) == "-ws" && n + 1 < argc) {
            ws = UTILS::parseInt(argv[n + 1]);
        } else if (string(argv[n]) == "-sh" && n + 1 < argc) {
            sh = UTILS::parseInt(argv[n + 1]);
        } else if (string(argv[n]) == "-ds" && n + 1 < argc) {
            ds = UTILS::parseInt(argv[n + 1]);
        } else if (string(argv[n]) == "-nf" && n + 1 < argc) {
            nf = UTILS::parseInt(argv[n + 1]);
        }
    }

    if (verbose) {
        std::cout << "Input Folder: " << inputFolder << std::endl;
        std::cout << "Output Folder: " << outputFolder << std::endl;
    }

    // Ensure output directory exists
    filesystem::create_directories(outputFolder);

    // Process each WAV file in the input folder
    for (const auto& entry : filesystem::directory_iterator(inputFolder)) {
        if (entry.path().extension() == ".wav") {
            std::string inputFilePath = entry.path().string();
            std::string outputFilePath = outputFolder + "/" + entry.path().stem().string() + ".sig";

            SndfileHandle audioFile(inputFilePath);
            if (audioFile.error()) {
                std::cerr << "Error reading audio file: " << inputFilePath << std::endl;
                continue;
            }

            auto signature = UTILS::computeFFTSignature(audioFile, ws, sh, ds, nf);

            std::ofstream os(outputFilePath, std::ofstream::binary);
            if (!os) {
                std::cerr << "Warning: failed to open file to write: " << outputFilePath << std::endl;
                continue;
            }

            os.write(reinterpret_cast<const char*>(signature.data()), static_cast<long>(signature.size()));

            if (verbose) {
                std::cout << "Processed " << inputFilePath << " -> " << outputFilePath << std::endl;
            }
        }
    }
}