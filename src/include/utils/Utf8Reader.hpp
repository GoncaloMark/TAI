#pragma once

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <string>
#include <utility>
#include <vector>
#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include "utf8Character.hpp"


namespace UTF8 {


    class Utf8Reader {

    private:
        std::filesystem::path filePath;
        std::ifstream file;

    public:
        explicit Utf8Reader(std::filesystem::path filePath) {
            this->filePath = std::move(filePath);
        };

        bool isOpen() {
            return file.is_open();
        };

        std::string cannotOpenFileExceptionMessage() {
            return "Failed to open file: " + filePath.string();
        };

        void openFile() {
            if(isOpen()) {
                throw std::runtime_error("File is already open: "+ filePath.string() + ".");
            }
            file.open(filePath, std::ios::binary);
            if(!isOpen()) {
                throw std::ios_base::failure(cannotOpenFileExceptionMessage());
            }
        };

        Utf8Character getNextCharacter() {
            if(!isOpen()) {
                throw std::ios_base::failure(cannotOpenFileExceptionMessage());
            }

            std::vector<char> charBytes;
            char byte;
            unsigned int i, charSize, charPosition = file.tellg();

            if(!file.get(byte)) {
                Utf8Character character("", charPosition);
                return character;
            }
            charBytes.push_back(byte);

            charSize = getCharLength(byte);
            for(i = 1; i < charSize; i++) {
                if(!file.get(byte)) {
                    throw std::ios_base::failure("File is corrupted or cannot be read: " + filePath.string());
                }
                charBytes.push_back(byte);
            }

            std::string utf8Character(charBytes.begin(), charBytes.end());
            Utf8Character character(utf8Character, charPosition);
            return character;
        };

        unsigned int getCharLength(unsigned char byte) {
            if((byte & 0x80) == 0) // 0xxxxxxx
            {
                return 1;
            }
            else if ((byte & 0xE0) == 0xC0) // 110xxxxx
            {
                return 2;
            }
            else if ((byte & 0xF0) == 0xE0) // 1110xxxx
            {
                return 3;
            }
            else if ((byte & 0xF8) == 0xF0) // 11110xxx
            {
                return 4;
            }
            else
            {
                throw std::ios_base::failure("File is corrupted or cannot be read: " + filePath.string());
            }

        };

        bool isEndOfFile() {
            if(!file.is_open()) {
                throw std::ios_base::failure("File is not open: " + filePath.string());
            }
            return file.eof();
        };

        void closeFile() {
            file.close();
        };

        Utf8Character getFirstChar() {
            Utf8Character firstChar;
            if(isOpen()) {
                unsigned int curPosition = getCurPos();
                file.seekg(0, std::ifstream::beg);

                firstChar = getNextCharacter();

                file.seekg(curPosition, std::ifstream::beg);
            } else {
                openFile();

                firstChar = getNextCharacter();

                closeFile();
            }

            return firstChar;
        };

        unsigned int getFileSize() {
            unsigned int fileSize;
            if(isOpen()) {
                unsigned int curPosition = getCurPos();

                file.seekg(0, std::ifstream::end);
                fileSize = file.tellg();

                file.seekg(curPosition, std::ifstream::beg);
            } else {
                openFile();

                file.seekg(0, std::ifstream::end);
                fileSize = file.tellg();

                closeFile();
            }
            return fileSize;
        };

        unsigned int getCurPos() {
            return file.tellg();
        };


    };


} // UTF8

