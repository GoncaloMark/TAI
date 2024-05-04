//
// Created by renan on 30/04/24.
//

#pragma once

#include <iostream>
#include <filesystem>
#include <unordered_set>

namespace UTILS {

    std::unordered_set<char> generateAsciiAlphabet() {
        std::unordered_set<char> alphabet;
        for(unsigned int i = 0; i < 128; ++i) {
            alphabet.insert(static_cast<char>(i));
        }
        return alphabet;
    }

    size_t getFileSizeBytes(const std::filesystem::path& filePath) {
        try {
            std::uintmax_t fileSize = std::filesystem::file_size(filePath);
            return static_cast<size_t>(fileSize);
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error accessing file: " << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    size_t getFileSize(const std::filesystem::path& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "File is not open or accessible" << std::endl;
            exit(EXIT_FAILURE);
        }

        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        file.close();
        return size;
    }
}
