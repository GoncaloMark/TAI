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

    /* Specific functions to our problems */

    std::vector<std::vector<std::string>> readCSV(const std::filesystem::path& filename) {
        csv::CSVReader reader(filename.string());
        std::vector<std::vector<std::string>> data;

        for (auto& row: reader) {
            std::vector<std::string> rowData;
            std::string text = row[0].get<std::string>();
            std::string label = row[1].get<std::string>();
            if(label != "0" && label != "1") {
                std::cout << "Error here! " << label << std::endl;
                exit(EXIT_FAILURE);
            }
            rowData.push_back(text);
            rowData.push_back(label);
            data.push_back(rowData);
        }

        return data;
    }

    std::vector<std::vector<std::string>> filter(const std::vector<std::vector<std::string>>& data, const std::string& label) {
        std::vector<std::vector<std::string>> filteredData;
        for(const auto& row: data) {
            if(row[1] == label) {
                filteredData.push_back(row);
            }
        }
        return filteredData;
    }

    std::vector<std::string> getInput(const std::vector<std::vector<std::string>>& data) {
        std::vector<std::string> input;
        for (const auto& row : data) {
            // Assuming the input is in the first column
            input.push_back(row[0]);
        }
        return input;
    }

    std::vector<int> getOutput(const std::vector<std::vector<std::string>>& data) {
        std::vector<int> output;
        int value;
        for (const auto& row : data) {
            value = stoi(row[1]);
            output.push_back(value);
        }
        return output;
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