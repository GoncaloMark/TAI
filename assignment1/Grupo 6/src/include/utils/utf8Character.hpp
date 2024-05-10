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
    class Utf8Character {

    private:
        std::string character;
        uint32_t position{};
    public:

        explicit Utf8Character() = default;
        explicit Utf8Character(std::string character, uint32_t position): character(std::move(character)), position(position) {};

        [[nodiscard]] const std::string &getCharacter() const {
            return character;
        }

        [[nodiscard]] uint32_t getPosition() const {
            return position;
        }

        static std::string convertToStr(const std::vector<UTF8::Utf8Character>& lst) {

            std::string str;
            for(const auto & character : lst) {
                str += character.getCharacter();
            }
            return str;
        }

    };
}
