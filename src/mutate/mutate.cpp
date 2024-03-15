#include "mutate.hpp"

namespace Mutate{
    uint32_t Mutator::mutateChar(uint32_t c){
        if (alphabetVec.size() <= 1) {
            return c;
        }

        uint32_t newChar = alphabetVec[rand() % alphabetVec.size()];
        while (newChar == c) {
            newChar = alphabetVec[rand() % alphabetVec.size()];
        }
        return newChar;
    }

    void Mutator::printAlphabet(){
        for(auto& character : alphabet){
            std::cout << "0x" << character << "\n";
        }
    }

    void Mutator::mutateFile(const size_t bufferSize = 1024){
        srand(static_cast<unsigned>(time(nullptr)));

        std::ofstream fileOutput(outputFilePath, std::ofstream::binary);

        if (!fileOutput.is_open()) {
            std::cerr << "ERROR::FILE_NOT_SUCCESFULLY_OPENED" << '\n';
            exit(EXIT_FAILURE);
        }

        std::cout << "Mutating File..." << std::endl;

        std::string outputBuffer;
        outputBuffer.reserve(bufferSize * 1024); 

        decoder.readAll([&](uint32_t character){
            double rnd = static_cast<double>(rand()) / RAND_MAX;
            uint32_t current = character;

            if (rnd < mutationProbability && alphabet.find(character) != alphabet.end()) {
                current = mutateChar(character);
            }
            
            outputBuffer.append(decoder.encode(current));

            if (outputBuffer.size() >= (bufferSize * 1024)) {
                fileOutput.write(outputBuffer.data(), outputBuffer.size());
                outputBuffer.clear();  // Clear the buffer after writing
            }
        });

        if (!outputBuffer.empty()) {
            fileOutput.write(outputBuffer.data(), outputBuffer.size());
            outputBuffer.clear();
        }

        std::cout << "Successfully Mutated File" << std::endl;

        fileOutput.close();
    }
}
