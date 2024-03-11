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


namespace UTF8 {

    class Utf8Reader {

    private:
        std::filesystem::path filePath;
        std::ifstream file;

    public:
        explicit Utf8Reader(std::filesystem::path filePath) {
            this->filePath = std::move(filePath);
        };

        std::filesystem::path getFilePath() {
            return filePath;
        };

        void openFile();
        std::string getNextCharacter();
        unsigned int getCharLength(char character);
        bool isEndOfFile();
        void closeFile();
    };


} // UTF8

