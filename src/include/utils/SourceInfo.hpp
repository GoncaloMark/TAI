#pragma once

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <string>
#include <array>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <set>
#include <utility>

#include "utf8Parser.hpp"
#include "parser.hpp"

namespace FILEINFO {

    class SourceInfo {

    public:
        explicit SourceInfo(std::set<uint32_t> charSet, size_t sourceSize): charSet(std::move(charSet)), sourceSize(sourceSize) {};

        std::set<uint32_t> alphabet() {
            return charSet;
        };

        [[nodiscard]] size_t getSourceSize() const {
            return sourceSize;
        };

        static FILEINFO::SourceInfo getInformation(Parser &parser) {
            SourceInfoBuilder builder;
            parser.readAll([&builder](uint32_t symbol) { builder.callbackFunction(symbol);});

            FILEINFO::SourceInfo sourceInfo(builder.charSet, builder.sourceSize);
            return sourceInfo;
        };

    private:
        std::set<uint32_t> charSet;
        size_t sourceSize;

        class SourceInfoBuilder {
        public:

            std::set<uint32_t> charSet;
            size_t sourceSize;

            explicit SourceInfoBuilder() {
                sourceSize = 0;
            };

            ~SourceInfoBuilder() = default;

            void callbackFunction(uint32_t symbol) {
                charSet.insert(symbol);
                sourceSize++;
            };

        };
    };

}

