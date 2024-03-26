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

#include "utf8Parser.hpp"
#include "parser.hpp"

namespace FILEINFO {

    class AlphabetBuilder {

    public:
        explicit AlphabetBuilder() = default;

        void getAlphabetCallback(uint32_t symbol) {
            alphabet.insert(symbol);
        };

        std::set<uint32_t> getAlphabet() {
            return alphabet;
        };

    private:
        std::set<uint32_t> alphabet;
    };

}

