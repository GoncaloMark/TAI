#include <iostream>
#include <filesystem>
#include <vector>
#include <sndfile.hh>

namespace fs = std::filesystem;

void createSegments(const std::string& inputFilePath, const std::string& outputDir, int segmentDuration) {
    SndfileHandle fileHandle(inputFilePath);
    if (fileHandle.error()) {
        std::cerr << "Error reading audio file: " << inputFilePath << std::endl;
        return;
    }

    int sampleRate = fileHandle.samplerate();
    int channels = fileHandle.channels();
    int framesPerSegment = segmentDuration * sampleRate;

    std::vector<short> buffer(framesPerSegment * channels);
    int segmentNumber = 0;
    int readCount = 0;

    while ((readCount = fileHandle.readf(buffer.data(), framesPerSegment)) > 0) {
        std::string outputFilePath = outputDir + "/" + fs::path(inputFilePath).stem().string() +
                                     "_segment_" + std::to_string(segmentNumber++) + ".wav";

        SndfileHandle outFile(outputFilePath, SFM_WRITE, fileHandle.format(), channels, sampleRate);
        if (outFile.error()) {
            std::cerr << "Error writing segment file: " << outputFilePath << std::endl;
            continue;
        }

        outFile.writef(buffer.data(), readCount);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <input_directory> <output_directory> <segment_duration>" << std::endl;
        return 1;
    }

    std::string inputDir = argv[1];
    std::string outputDir = argv[2];
    int segmentDuration = std::stoi(argv[3]);

    for (const auto& entry : fs::directory_iterator(inputDir)) {
        if (entry.path().extension() == ".wav") {
            createSegments(entry.path().string(), outputDir, segmentDuration);
        }
    }

    return 0;
}
