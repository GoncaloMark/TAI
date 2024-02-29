#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/argparser/argparser.hpp"
#include "mutate.hpp"

int main(int argc, char** argv){
    argc--;
    argv++;

    ArgParser::ArgParser parser(argc, argv);

    parser.registerArgType<double>("--probability");
    parser.registerArgType<std::filesystem::path>("--inputFileName");
    parser.registerArgType<std::filesystem::path>("--outputFileName");

    parser.ParseArgs();

    double prob = parser.GetArgValue<double>("--probability") / 100.0;
    std::filesystem::path inputFileName = parser.GetArgValue<std::filesystem::path>("--inputFileName");
    std::filesystem::path outputFileName = parser.GetArgValue<std::filesystem::path>("--outputFileName");

    // Seed the randomness
    srand(static_cast<unsigned>(time(nullptr)));

    const size_t bufferSize = 1024; 
    char buffer[bufferSize];

    std::stringstream sequenceStream;

    std::ifstream fileSource;
    std::ofstream fileOutput;

    fileSource.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fileOutput.exceptions(std::ofstream::failbit | std::ofstream::badbit);

    try {
        fileSource.open(inputFileName, std::ifstream::binary);
        if (!fileSource.is_open()) {
            std::cerr << "ERROR::SOURCE::FILE_NOT_SUCCESFULLY_OPENED" << '\n';
            throw std::ios_base::failure("ERROR::SOURCE::FILE_NOT_SUCCESFULLY_OPENED");
        }

        fileOutput.open(outputFileName, std::ofstream::binary);
        if (!fileOutput.is_open()) {
            std::cerr << "ERROR::OUTPUT::FILE_NOT_SUCCESFULLY_OPENED" << '\n';
            throw std::ios_base::failure("ERROR::OUTPUT::FILE_NOT_SUCCESFULLY_OPENED");
        }

        while (fileSource.read(buffer, bufferSize) || fileSource.gcount() > 0) {
            for (std::streamsize i = 0; i < fileSource.gcount(); ++i) {
                double rnd = static_cast<double>(rand()) / RAND_MAX;
                if (rnd < prob && (buffer[i] == 'A' || buffer[i] == 'C' || buffer[i] == 'G' || buffer[i] == 'T')) {
                    buffer[i] = mutateChar(buffer[i]);
                }
            }
            fileOutput.write(buffer, fileSource.gcount());
            if (fileSource.peek() == EOF) break;
        }

        std::cout << "Successfully Mutated " << inputFileName << " File" << std::endl;
    } catch (const std::ios_base::failure& e) {
        std::cerr << "ERROR::INPUT_FILE_NOT_SUCCESFULLY_PROCESSED::" << e.what() << '\n';
        fileSource.close();
        fileOutput.close();
        return EXIT_FAILURE;
    }

    fileSource.close();
    fileOutput.close();

    return EXIT_SUCCESS;
}