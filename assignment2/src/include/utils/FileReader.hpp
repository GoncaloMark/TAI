#pragma once

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <string>
#include <array>
#include <cstdint>
#include <filesystem>
#include <functional>


namespace READER {

    class FileReader {
    public:
        static void read(const std::filesystem::path& filePath, const std::function<void(char)>& callback);
    private:
    };

} // READER
