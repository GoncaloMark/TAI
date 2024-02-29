#pragma once

#include <filesystem>
#include <stdlib.h>
#include <unordered_set>
#include <cstdint>
#include "utils/utf8Parser.hpp"

namespace Mutate {
    char mutateChar(char c);

    class Mutator {
        public:
            Mutator(std::filesystem::path inputFilePath, std::filesystem::path outputFilePath, double mutationProbability, UTF8::Utf8Parser utf8Decoder) : inputFilePath(inputFilePath), outputFilePath(outputFilePath), mutationProbability(mutationProbability), decoder(utf8Decoder){};
            void MutateFile();

            /// @brief Prints the alphabet read by the decoder.
            void PrintAlphabet();

            /// @brief Method reads the alphabet from the file.
            void ReadAlphabet();

        private:
            std::filesystem::path inputFilePath;
            std::filesystem::path outputFilePath;
            double mutationProbability;
            UTF8::Utf8Parser decoder;
    };
}
