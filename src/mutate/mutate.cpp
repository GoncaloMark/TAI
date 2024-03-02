#include "mutate.hpp"

namespace Mutate{
    uint32_t Mutator::mutateChar(uint32_t c){
        std::vector<uint32_t> alphabetVec(decoder.getAlphabet().begin(), decoder.getAlphabet().end());

        if (alphabetVec.size() <= 1) {
            return c;
        }

        uint32_t newChar = alphabetVec[rand() % alphabetVec.size()];
        while (newChar == c) {
            newChar = alphabetVec[rand() % alphabetVec.size()];
        }
        return newChar;
    }

    void Mutator::ReadAlphabet(){
        decoder.readFile(inputFilePath);
    }

    void Mutator::PrintAlphabet(){
        decoder.printAlphabet();
    }

    void Mutator::MutateFile(){
        srand(static_cast<unsigned>(time(nullptr)));

        const size_t bufferSize = 1024;
        std::vector<char> buffer(bufferSize);
        std::ifstream fileSource(inputFilePath, std::ifstream::binary);
        std::ofstream fileOutput(outputFilePath, std::ofstream::binary);

        if (!fileSource.is_open() || !fileOutput.is_open()) {
            std::cerr << "ERROR::FILE_NOT_SUCCESFULLY_OPENED" << '\n';
            exit(EXIT_FAILURE);
        }

        while (fileSource.read(buffer.data(), bufferSize) || fileSource.gcount() > 0) {
            std::streamsize count = fileSource.gcount();
            std::string outputBuffer; // Use a string to accumulate output

            for (std::streamsize i = 0; i < count; ) {
                double rnd = static_cast<double>(rand()) / RAND_MAX;
                size_t len = decoder.getCharLength(buffer[i]);
                std::string decodedChar(buffer.begin() + i, buffer.begin() + i + len);

                uint32_t hexChar = decoder.toHex(decodedChar);

                if (rnd < mutationProbability && decoder.getAlphabet().find(hexChar) != decoder.getAlphabet().end()) {
                    uint32_t mutatedHexChar = mutateChar(hexChar);
                    decodedChar = decoder.encode(mutatedHexChar);
                }

                outputBuffer += decodedChar; // Append the original or mutated character
                i += len; // Move to the next character
            }

            fileOutput.write(outputBuffer.data(), outputBuffer.size());
        }

        std::cout << "Successfully Mutated " << inputFilePath << " File" << std::endl;

        fileSource.close();
        fileOutput.close();
    }
}
