#include "helpers.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <unordered_set>

namespace UTILS {
    std::unordered_set<char> generateAsciiAlphabet() {
        std::unordered_set<char> alphabet;
        for(unsigned int i = 0; i < 128; ++i) {
            alphabet.insert(static_cast<char>(i));
        }
        return alphabet;
    }

    std::string escapeString(const std::string& str) {
        std::string escapedStr;
        for (char c : str) {
            switch (c) {
                case '\n':
                    escapedStr += "\\n";
                    break;
                case '\t':
                    escapedStr += "\\t";
                    break;
                case '\r':
                    escapedStr += "\\r";
                    break;
                    // Add more cases for other special characters if needed
                default:
                    escapedStr += c;
            }
        }
        return escapedStr;
    }

    std::string readText(const std::filesystem::path &filePath) {
        // Open the file
        std::ifstream file(filePath);

        // Check if the file is opened successfully
        if (!file.is_open()) {
            std::cerr << "Failed to open the file " << filePath << "!" << std::endl;
            throw std::runtime_error("Failed to open the file!");
        }
        std::string text((std::istreambuf_iterator<char>(file)),
                         (std::istreambuf_iterator<char>()));
        file.close();
        return text;
    }

}