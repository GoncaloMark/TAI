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
        unsigned int position{};
    public:

        explicit Utf8Character() = default;

        explicit Utf8Character(std::string character, unsigned int position) {
            this->character = std::move(character);
            this->position = position;
        };

        [[nodiscard]] const std::string &getCharacter() const {
            return character;
        }

        [[nodiscard]] unsigned int getPosition() const {
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
