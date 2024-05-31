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
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <numeric>
#include <vector>
#include <sndfile.hh>
#include <fftw3.h>
#include <sstream>
#include <bitset>

#define WS	1024	// Size of the window for computing the FFT
#define SH	256		// Window overlap
#define DS	4		// Down-sampling factor
#define NF	4		// Number of significant frequencies

using namespace std;

// Function to parse integer arguments safely
int parseArg(const char* arg) {
    char* end;
    errno = 0;
    long val = strtol(arg, &end, 10);
    if (errno != 0 || *end != '\0' || val < 0) {
        cerr << "Error: Invalid argument " << arg << endl;
        exit(EXIT_FAILURE);
    }
    return static_cast<int>(val);
}

SndfileHandle createAudioFile(const string& iFName) {
    SndfileHandle audioFile{iFName};
    if (audioFile.error()) {
        cerr << "Error: invalid audio file\n";
        exit(EXIT_FAILURE);
    }
    if (audioFile.channels() != 2) {
        cerr << "Error: currently supports only 2 channels\n";
        exit(EXIT_FAILURE);
    }
    if (audioFile.samplerate() != 44100) {
        cerr << "Error: currently supports only 44100 Hz of sample rate\n";
        exit(EXIT_FAILURE);
    }
    return audioFile;
}

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

// Function to compute the FFT signature
vector<unsigned char> computeFFTSig(const vector<short>& samples, int nFrames, int ws, int sh, int ds, int nf) {
    vector<unsigned char> binarySignature;

    fftw_complex in[ws] = {};
    fftw_complex out[ws] = {};

    fftw_plan plan = fftw_plan_dft_1d(ws, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    vector<double> power(ws/2);
    vector<unsigned> maxPowerIdx(ws/2);

    for(int n = 0 ; n <= (nFrames - ws * ds) / (sh * ds) ; ++n) {
        for(int k = 0 ; k < ws ; ++k) { // Convert to mono and down-sample
            in[k][0] = (int)samples[(n * (sh * ds) + k * ds) << 1] + samples[((n * (sh * ds) + k * ds) << 1) + 1];
            for(int l = 1 ; l < ds ; ++l) {
                in[k][0] += (int) samples[(n * (sh * ds) + k * ds + l) << 1] + samples[((n * (sh * ds) + k * ds + l) << 1) + 1];
            }
        }

        fftw_execute(plan);

        for(int k = 0 ; k < static_cast<int>(power.size()); ++k) {
            power[k] = out[k][0] * out[k][0] + out[k][1] * out[k][1];
        }

        iota(maxPowerIdx.begin(), maxPowerIdx.end(), 0);
        partial_sort(maxPowerIdx.begin(), maxPowerIdx.begin() + nf, maxPowerIdx.end(),[&power](int i, int j) { return power[i] > power[j]; });

        for(int i = 0 ; i < nf ; ++i) {
            // To store in a byte, truncate to a max of 255
            auto c = maxPowerIdx[i] > 255 ? 255 : maxPowerIdx[i];
            binarySignature.push_back(static_cast<unsigned char>(c));
        }

    }
    fftw_destroy_plan(plan);
    return binarySignature;
}

int main (int argc, char* argv[]) {

    // Declare variables
	bool verbose { false };
    string oFName;
    string iFName;
	int ws { WS };
	int sh { SH };
	int ds { DS };
	int nf { NF };

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
            ws = parseArg(argv[n + 1]);
        } else if (string(argv[n]) == "-sh" && n + 1 < argc) {
            sh = parseArg(argv[n + 1]);
        } else if (string(argv[n]) == "-ds" && n + 1 < argc) {
            ds = parseArg(argv[n + 1]);
        } else if (string(argv[n]) == "-nf" && n + 1 < argc) {
            nf = parseArg(argv[n + 1]);
        }
    }
    iFName = argv[argc-1];

    // Start Program execution
	SndfileHandle audioFile = createAudioFile(iFName);

    if(verbose) {
        printf("Sample rate : %d\n",  audioFile.samplerate());
        printf("Channels    : %d\n",  audioFile.channels());
        printf("Frames      : %ld\n", (long int)audioFile.frames());
    }

    ofstream os;
	if(!oFName.empty()) {
		os.open(oFName, ofstream::binary);
		if(!os) {
			cerr << "Warning: failed to open file to write\n";
		}
	}

    // Read audio samples
    vector<short> samples(audioFile.frames() * audioFile.channels());
    audioFile.readf(samples.data(), audioFile.frames());
    int nFrames = static_cast<int>(audioFile.frames());

    auto sig = computeFFTSig(samples, nFrames, ws, sh, ds, nf);
    os.write(reinterpret_cast<const char*>(sig.data()), sig.size());

	return EXIT_SUCCESS;
}

