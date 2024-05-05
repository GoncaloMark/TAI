//
// Created by renan on 30/04/24.
//

#pragma once

#include <iostream>
#include <filesystem>
#include <unordered_set>
#include "csv.hpp"

namespace UTILS {

    std::unordered_set<char> generateAsciiAlphabet();
    std::string escapeString(const std::string& str);
    size_t getFileSizeBytes(const std::filesystem::path& filePath);
    size_t getFileSize(const std::filesystem::path& filePath);

    bool fileExists(const std::filesystem::path& path);

    void printCSV(const std::vector<std::vector<std::string>>& data) ;

    /* Specific functions to our problems */

    std::vector<std::vector<std::string>> readCSV(const std::filesystem::path& filename);
    std::vector<std::vector<std::string>> filter(const std::vector<std::vector<std::string>>& data, const std::string& label);
    std::vector<std::string> getInput(const std::vector<std::vector<std::string>>& data);
    std::vector<int> getOutput(const std::vector<std::vector<std::string>>& data);

}
