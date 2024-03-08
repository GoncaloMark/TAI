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
            Mutator(std::filesystem::path inputFilePath, std::filesystem::path outputFilePath, double mutationProbability, Parser& decoder) : inputFilePath(inputFilePath), outputFilePath(outputFilePath), mutationProbability(mutationProbability), decoder(decoder){};

            /// @brief Mutates the input file outputting to output file.
            void MutateFile();

            /// @brief Prints the alphabet read by the decoder.
            void PrintAlphabet();

            /// @brief Method reads the alphabet from the file.
            void ReadAlphabet();

        private:
            std::filesystem::path inputFilePath;
            std::filesystem::path outputFilePath;
            double mutationProbability;
            Parser& decoder;

            uint32_t mutateChar(uint32_t c);
    };
}
