#include "mutate.hpp"

namespace Mutate{
    uint32_t Mutator::mutateChar(uint32_t c){
        std::vector<uint32_t> alphabetVec(decoder.getAlphabet().begin(), decoder.getAlphabet().begin());

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

    //TODO: Fix the encoding and decoding of characters in real time for the mutation!
    void Mutator::MutateFile(){
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
            fileSource.open(inputFilePath, std::ifstream::binary);
            if (!fileSource.is_open()) {
                std::cerr << "ERROR::SOURCE::FILE_NOT_SUCCESFULLY_OPENED" << '\n';
                throw std::ios_base::failure("ERROR::SOURCE::FILE_NOT_SUCCESFULLY_OPENED");
            }

            fileOutput.open(outputFilePath, std::ofstream::binary);
            if (!fileOutput.is_open()) {
                std::cerr << "ERROR::OUTPUT::FILE_NOT_SUCCESFULLY_OPENED" << '\n';
                throw std::ios_base::failure("ERROR::OUTPUT::FILE_NOT_SUCCESFULLY_OPENED");
            }

            while (fileSource.read(buffer, bufferSize) || fileSource.gcount() > 0) {
                for (std::streamsize i = 0; i < fileSource.gcount(); ++i) {
                    double rnd = static_cast<double>(rand()) / RAND_MAX;
                    if (rnd < mutationProbability && decoder.getAlphabet().find(static_cast<uint32_t>(buffer[i])) != decoder.getAlphabet().end()) {
                        buffer[i] = Mutate::Mutator::mutateChar(buffer[i]);
                    }
                }
                fileOutput.write(buffer, fileSource.gcount());
                if (fileSource.peek() == EOF) break;
            }

            std::cout << "Successfully Mutated " << inputFilePath << " File" << std::endl;
        } catch (const std::ios_base::failure& e) {
            std::cerr << "ERROR::INPUT_FILE_NOT_SUCCESFULLY_PROCESSED::" << e.what() << '\n';
            fileSource.close();
            fileOutput.close();
            exit(EXIT_FAILURE);
        }

        fileSource.close();
        fileOutput.close();
    }
}
