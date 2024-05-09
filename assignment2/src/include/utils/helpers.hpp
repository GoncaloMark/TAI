//
// Created by renan on 30/04/24.
//

#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <unordered_set>

namespace UTILS {

    std::unordered_set<char> generateAsciiAlphabet();
    std::string escapeString(const std::string& str);
    std::string readText(const std::filesystem::path& filePath);

}
