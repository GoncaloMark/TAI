#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/argparser/argparser.hpp"

char mutateChar(char c) {
    char chars[] = {'A', 'C', 'G', 'T'};
    char newChar = chars[rand() % 4];
    while (newChar == c) {
        newChar = chars[rand() % 4];
    }
    return newChar;
}

int main(int argc, char** argv){
    argc--;
    argv++;

    if(argc < 1){
        std::cerr << "ERROR::MISSING_ARGUMENTS" << '\n';

        std::cout << "Example:\n" \
        << "mutate file.txt 20:\n" \
        << std::endl;
        return EXIT_FAILURE;
    }

    double prob = atof(argv[1]) / 100.0; 
    srand(static_cast<unsigned>(time(nullptr)));

    const size_t bufferSize = 1024; 
    char buffer[bufferSize];

    std::stringstream sequenceStream;

    std::ifstream fileSource;
    std::ofstream fileOutput;

    fileSource.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fileOutput.exceptions(std::ofstream::failbit | std::ofstream::badbit);

    try {
        fileSource.open(argv[0], std::ifstream::binary);
        if (!fileSource.is_open()) {
            std::cerr << "ERROR::SOURCE::FILE_NOT_SUCCESFULLY_OPENED" << '\n';
            throw std::ios_base::failure("ERROR::SOURCE::FILE_NOT_SUCCESFULLY_OPENED");
        }

        fileOutput.open("mutated_chry.txt", std::ofstream::binary);
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

        std::cout << "Successfully Mutated " << argv[0] << " File" << std::endl;
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