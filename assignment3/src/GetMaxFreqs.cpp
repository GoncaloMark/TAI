//
// Armando J. Pinho, ap@ua.pt, 2016-2020
//
// See:
// http://www.mega-nerd.com/libsndfile
// http://www.fftw.org
//
// In linux, compile with:
// g++ -W -Wall -std=c++11 -o GetMaxFreqs GetMaxFreqs.cpp -lsndfile -lfftw3 -lm
//
// It should accept .wav and .flac audio files, stereo, sampled at 44100 Hz, 16 bits
//
// Example:
// GetMaxFreqs -w test.freqs test.wav
//
// File test.freqs will contain the "signature" of the audio file test.wav
//
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sndfile.hh>
#include <bitset>
#include "Helpers.hpp"
#include "Constants.hpp"

using namespace std;

// Function to print help message
void helpMessage() {
    cerr << "Usage: GetMaxFreqs [ -v (verbose) ]" << endl;
    cerr << "                   [ -w freqsFile ]" << endl;
    cerr << "                   [ -ws winSize ]" << endl;
    cerr << "                   [ -sh shift ]" << endl;
    cerr << "                   [ -ds downSampling ]" << endl;
    cerr << "                   [ -nf nFreqs ]" << endl;
    cerr << "                   AudioFile" << endl;
}

int main (int argc, char* argv[]) {

    // Declare variables
	bool verbose = false;
    string oFName;
    string iFName;
	int ws = Constants::WINDOW_SIZE;
	int sh = Constants::SHIFT;
	int ds = Constants::DOWNSAMPLING;
	int nf = Constants::NUM_FREQS;

	if(argc < 2) {
        helpMessage();
		return EXIT_FAILURE;
	}
    // Parse arguments
    for (int n = 1; n < argc; n++) {
        if (string(argv[n]) == "-v") {
            verbose = true;
        } else if (string(argv[n]) == "-w" && n + 1 < argc) {
            oFName = argv[n + 1];
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
    iFName = argv[argc-1];

    // Start Program execution
    SndfileHandle audioFile{iFName};
    if(verbose) {
        printf("Sample rate : %d\n",  audioFile.samplerate());
        printf("Channels    : %d\n",  audioFile.channels());
        printf("Frames      : %ld\n", (long int)audioFile.frames());
    }
    auto sig = UTILS::computeFFTSignature(audioFile, ws, sh, ds, nf);
    ofstream os;
    if(!oFName.empty()) {
        os.open(oFName, ofstream::binary);
        if(!os) {
            cerr << "Warning: failed to open file to write\n";
        }
    }
    os.write(reinterpret_cast<const char*>(sig.data()), static_cast<long>(sig.size()));

	return EXIT_SUCCESS;
}

