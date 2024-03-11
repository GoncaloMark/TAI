//
// Created by renan on 11/03/24.
//

#include "Utf8Reader.hpp"

namespace UTF8 {


    void Utf8Reader::openFile() {
        file.open(filePath, std::ios::binary);

        if(!file.is_open()) {
            throw std::ios_base::failure("Utf8Reader::openFile: Failed to open file: " + filePath.string() + ".");
        }
    }

    std::string Utf8Reader::getNextCharacter() {
        if(!file.is_open()) {
            throw std::ios_base::failure("Utf8Reader::getNextCharacter: Failed to open file: " + filePath.string() + ".");
        }

        std::vector<char> charBytes;

        char byte;
        if(!file.get(byte)) {
            return "";
        }
        charBytes.push_back(byte);

        unsigned int charSize = getCharLength(byte);
        std::cout << "-" << charSize << std::endl;
        for(unsigned int i = 1; i < charSize; i++) {
            if(!file.get(byte)) {
                throw std::ios_base::failure("File is corrupted or cannot be read: " + filePath.string());
            }
            charBytes.push_back(byte);
        }

        std::string utf8Character(charBytes.begin(), charBytes.end());
        return utf8Character;
    }

    unsigned int Utf8Reader::getCharLength(char character) {
        unsigned char byte = character;
        if((byte & 0x80) == 0) { // 0xxxxxxx
            return 1;
        } else if ((byte & 0xE0) == 0xC0) { // 110xxxxx
            return 2;
        } else if ((byte & 0xF0) == 0xE0) { // 1110xxxx
            return 3;
        } else if ((byte & 0xF8) == 0xF0) { // 11110xxx
            return 4;
        } else {
            throw std::ios_base::failure("Utf8Reader::getCharLength: File is corrupted or cannot be read: " + filePath.string());
        }

    }

    bool Utf8Reader::isEndOfFile() {
        if(!file.is_open()) {
            throw std::ios_base::failure("File is not open: " + filePath.string());
        }
        return file.eof();
    }

    void Utf8Reader::closeFile() {
        file.close();
    }
}