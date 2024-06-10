#pragma once

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <string>
#include <array>
#include <cstdint>
#include <filesystem>
#include <functional>

#include "parser.hpp"

namespace UTF8 {
    class Utf8Parser : public Parser {
        public:
            /// @brief We open the file on the constructor.
            explicit Utf8Parser(std::filesystem::path filePath, size_t bufferSize) : Parser(bufferSize), file(filePath, std::ios::binary) {
                if (!file) {
                    std::cerr << "Failed to open file: " << filePath << std::endl;
                    throw std::runtime_error("Failed to open file");
                }
            };

            /// @brief Destructor closes the file.
            ~Utf8Parser() {
                std::cout << "Destroying Parser..." << std::endl;
                if (file.is_open()) {
                    file.close();
                }
            }

            /// @brief Reads and parses the file applying user defined callback to the hex representation of a character.
            /// @param callback user defined callback for handling the character processing.
            void readAll(std::function<void(uint32_t)> callback) override;

            /// @brief This function reads chunk into the specified buffers. 
            /// @param buffers Buffer vector containing the buffers.
            /// @param bufferSize Size to be read into buffer.
            /// @return Boolean, true in case of success, false in case of unsuccess.
            bool readChunk(std::vector<uint32_t>& buffer, size_t bufferSize) override;

            /// @brief Function to encode the hexadecimal back to UTF8
            /// @param character character to be encoded.
            /// @return std::string representing the encoded character.
            std::string encode(uint32_t character) override;

            /// @brief Private method to get the getSourceSize in bytes of the character being read.
            /// @param character Character to be analysed.
            /// @return Returns a size_t representing getSourceSize in bytes of character.
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

        private:
            std::ifstream file;
            std::streampos readPosition;
    };
}