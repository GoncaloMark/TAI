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
#include <unordered_map>

#include "utf8Parser.hpp"
#include "parser.hpp"
#include "Helpers.hpp"

namespace FILEINFO {

    class SourceInfo {

    public:
        explicit SourceInfo(std::unordered_map<uint32_t, size_t> frequency): charCount(std::move(frequency)) {
            sourceSize = 0;
            for(const auto& pair: charCount) {
                charSet.insert(pair.first);
                sourceSize += pair.second;
            }

            totalBits = 0.0;
            for(const auto& symbol: charSet) {
                charFreq[symbol] = ((double) charCount[symbol])/((double) sourceSize);
                charEntropy[symbol] = UTILS::Helpers::calculateEntropy(charFreq[symbol]);
                totalBits += charEntropy[symbol];
            }
        };

        std::set<uint32_t> alphabet() {
            return charSet;
        };

        [[nodiscard]] size_t size() const {
            return sourceSize;
        };

        double getTotalBits() const {
            return totalBits;
        }

        [[maybe_unused]] std::unordered_map<uint32_t, size_t> counter() {
            return charCount;
        }

        std::unordered_map<uint32_t, double> getFrequency() {
            return charFreq;
        }

        std::unordered_map<uint32_t, double> getEntropy() {
            return charEntropy;
        }

        static FILEINFO::SourceInfo getInformation(Parser &parser) {
            SourceInfoBuilder builder;
            parser.readAll([&builder](uint32_t symbol) { builder.callbackFunction(symbol);});

            return FILEINFO::SourceInfo(builder.frequency);
        };

    private:
        std::unordered_map<uint32_t, size_t> charCount;
        std::unordered_map<uint32_t, double> charFreq;
        std::unordered_map<uint32_t, double> charEntropy;
        std::set<uint32_t> charSet;
        size_t sourceSize;
        double totalBits;

        class SourceInfoBuilder {
        public:
            std::unordered_map<uint32_t, size_t> frequency;
            explicit SourceInfoBuilder() = default;
            ~SourceInfoBuilder() = default;

            void callbackFunction(uint32_t symbol) {
                if(frequency.find(symbol) == frequency.end()) { // If it does not contain symbol
                    frequency[symbol] = 0;
                }
                frequency[symbol]++;
            };

        };
    };

}

