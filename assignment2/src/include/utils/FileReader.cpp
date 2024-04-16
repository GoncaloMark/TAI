#include "FileReader.hpp"

namespace READER {

    void FileReader::read(const std::filesystem::path& filePath, const std::function<void(char)>& callback) {
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
} // READER