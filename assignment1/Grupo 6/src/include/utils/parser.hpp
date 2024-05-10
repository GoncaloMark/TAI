#pragma once

#include <unordered_set>
#include <string>
#include <cstdint>
#include <filesystem>
#include <functional>

/// @brief Abstract class stipulates the contract for Parsers, be it ASCII, UTF-8, UTF-16, etc...
class Parser {
    public:
        explicit Parser(size_t bufferSize) : bufferSize(bufferSize) {};
        virtual bool readChunk(std::vector<uint32_t>& buffer, size_t bufferSize) = 0;
        virtual void readAll(std::function<void(uint32_t)> callback) = 0;
        virtual std::string encode(uint32_t character) = 0;
        virtual size_t getCharLength(char character) = 0;
        virtual uint32_t toHex(const std::string& character) = 0;

    protected:
        size_t bufferSize;
        std::string leftover;
};