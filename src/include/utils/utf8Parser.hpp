#pragma once

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <string>
#include <vector>
#include <cstdint>
#include <filesystem>

#include "parser.hpp"

// TODO Use move semantics for passing around the strings.
namespace UTF8 {
    class Utf8Parser : public Parser {
        public:
            Utf8Parser(size_t bufferSize) : Parser(bufferSize) {};

            /// @brief Reads and parses the file to get the alphabet (unique characters) into an unordered_set.
            /// @param filePath Path to file in the system.
            void readFile(std::filesystem::path filePath) override;

            /// @brief Prints the hexadecimal code points stored in the unordered_set.
            void printAlphabet() override {
                for (const auto& character : characters) {
                    std::cout << "0x" << std::hex << character << std::endl;
                }
            }

            /// @brief Function to encode the hexadecimal back to UTF8
            /// @param character character to be encoded.
            /// @return std::string representing the encoded character.
            std::string encode(uint32_t character) override;

            /// @brief Getter for the alphabet.
            /// @return Alphabet unordered_set constant reference (read-only).
            const std::unordered_set<uint32_t>& getAlphabet() const override {
                return characters;
            }

            /// @brief Private method to get the size in bytes of the character being read.
            /// @param character Character to be analysed.
            /// @return Returns a size_t representing size in bytes of character.
            size_t getCharLength(char character) override {
                unsigned char byte = character;
                if ((byte & 0x80) == 0) return 1; // 0xxxxxxx
                if ((byte & 0xE0) == 0xC0) return 2; // 110xxxxx
                if ((byte & 0xF0) == 0xE0) return 3; // 1110xxxx
                if ((byte & 0xF8) == 0xF0) return 4; // 11110xxx
                return 1; // Default to 1 for continuation bytes or in case of invalid input
            }

            /// @brief This method takes the utf8 string representation and builds the uint32_t format.
            /// @param character UTF8 string format of the bytes.
            /// @return Unsigned integer with 32 bits representing the bytes in hexadecimal notation.
            uint32_t toHex(const std::string& character) override;
    };
}