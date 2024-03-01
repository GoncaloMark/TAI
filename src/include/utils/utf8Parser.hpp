#pragma once

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <string>
#include <vector>
#include <cstdint>
#include <filesystem>

//TODO Decoder abstract class/interface.
namespace UTF8 {
    /// @brief Utility function takes the utf8 string representation and builds the uint32_t format.
    /// @param utf8 UTF8 string format of the bytes.
    /// @return Unsigned integer with 32 bits representing the bytes in hexadecimal notation.
    uint32_t utf8ToHex(const std::string& utf8);

    /// @brief This is a utility function used for converting characters to lower case if they're uppercase.
    /// @param utf8 Hexadecimal code of the character to be converted.
    /// @return Hexadecimal code for lowercase character.
    uint32_t utf8ToLower(uint32_t utf8);

    class Utf8Parser {
    public:
        Utf8Parser(size_t bufferSize) : bufferSize(bufferSize) {};

        /// @brief Reads and parses the file to get the alphabet (unique characters) into an unordered_set.
        /// @param filePath Path to file in the system.
        void readFile(std::filesystem::path filePath);

        /// @brief Prints the hexadecimal code points stored in the unordered_set.
        void printAlphabet() {
            for (const auto& character : characters) {
                std::cout << "0x" << std::hex << character << std::endl;
            }
        }

        /// @brief Function to encode the hexadecimal back to UTF8
        /// @param character character to be encoded.
        /// @return std::string representing the encoded character.
        std::string encodeUtf8(uint32_t character);

        //TODO MAKE THIS RETURN A REFERENCE OR SOMETHING SO IT DOESNT COPY!
        /// @brief Getter for the alphabet.
        /// @return Alphabet unordered_set.
        std::unordered_set<uint32_t> getAlphabet(){
            return characters;
        }

        /// @brief Private method to get the size in bytes of the character being read.
        /// @param ch Character to be analysed.
        /// @return Returns a size_t representing size in bytes of character.
        size_t getUtf8CharLength(char ch) {
            unsigned char byte = ch;
            if ((byte & 0x80) == 0) return 1; // 0xxxxxxx
            if ((byte & 0xE0) == 0xC0) return 2; // 110xxxxx
            if ((byte & 0xF0) == 0xE0) return 3; // 1110xxxx
            if ((byte & 0xF8) == 0xF0) return 4; // 11110xxx
            return 1; // Default to 1 for continuation bytes or in case of invalid input
        }

        private:
            std::unordered_set<uint32_t> characters;
            size_t bufferSize;
    };
}