#pragma once

#include <filesystem>
#include <stdlib.h>
#include <unordered_set>
#include <cstdint>
#include "utils/parser.hpp"
#include "utils/utf8Parser.hpp"

namespace Mutate {
    class Mutator {
        public:
            explicit Mutator(std::filesystem::path outputFilePath, const double mutationProbability, Parser& decoder) : outputFilePath(std::move(outputFilePath)), mutationProbability(mutationProbability), decoder(decoder){
                std::cout << "Reading Alphabet..." << std::endl;
                decoder.readAll([&](uint32_t character) {
                    if (character != 0x9 && character != 0xA && character != 0xD && character != 0x20){
                        alphabet.insert(character);
                    }
                });

                alphabetVec = std::vector<uint32_t>(alphabet.begin(), alphabet.end());
            };

            /// @brief Mutates the input file outputting to output file.
            /// @param bufferSize bufsize from cmd line.
            void mutateFile(const size_t bufferSize);

            /// @brief Prints the alphabet read by the decoder.
            void printAlphabet();

        private:
            const std::filesystem::path outputFilePath;
            const double mutationProbability;
            std::unordered_set<uint32_t> alphabet;
            std::vector<uint32_t> alphabetVec;
            Parser& decoder;

            uint32_t mutateChar(uint32_t c);
    };
}
