#pragma once

#include <unordered_set>
#include <string>
#include <cstdint>
#include <filesystem>

/// @brief Abstract class stipulates the contract for Parsers, be it ASCII, UTF-8, UTF-16, etc...
class Parser {
    public:
        Parser(size_t bufferSize) : bufferSize(bufferSize) {};

        virtual void readAlphabet(std::filesystem::path filePath) = 0;
        virtual void printAlphabet() = 0;
        virtual std::string encode(uint32_t character) = 0;
        virtual const std::unordered_set<uint32_t>& getAlphabet() const = 0;
        virtual size_t getCharLength(char character) = 0;
        virtual uint32_t toHex(const std::string& character) = 0;

    protected:
        std::unordered_set<uint32_t> characters;
        size_t bufferSize;
};