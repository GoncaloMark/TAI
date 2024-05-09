//
// Created by renan on 05/05/24.
//
#include "helpers.hpp"

namespace UTILS {
    std::unordered_set<char> generateAsciiAlphabet() {
        std::unordered_set<char> alphabet;
        for(unsigned int i = 0; i < 128; ++i) {
            alphabet.insert(static_cast<char>(i));
        }
        return alphabet;
    }

    size_t hashContext(const std::string& context) {

        size_t hash = 0;
        for (char c : context) {
            hash = (hash * PRIME_NUMBER) + static_cast<size_t>(c);
        }
        return hash;
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

    bool fileExists(const std::filesystem::path& path) {
        std::ifstream file(path);
        return file.good();
    }

    void printCSV(const std::vector<std::vector<std::string>>& data) {
        for (const auto& row : data) {
            for (const auto& cell : row) {
                std::cout << cell << ",";
            }
            std::cout << std::endl;
        }
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

    void processFile(const std::filesystem::path &filePath, const std::function<void(char)> &callback) {
        std::ifstream file(filePath);
        if (!file || !file.is_open()) {
            std::cerr << "File is not open or accessible" << std::endl;
            exit(EXIT_FAILURE);
        }

        char curChar;
        while(file.get(curChar)) {
            callback(curChar);
        }

        file.close();
    }
}