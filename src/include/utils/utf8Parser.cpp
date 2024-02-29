#include "utf8Parser.hpp"

namespace UTF8 {
    uint32_t utf8ToCodepoint(const std::string& utf8) {
        std::vector<unsigned char> bytes(utf8.begin(), utf8.end());
        uint32_t codepoint = 0;
        size_t numBytes = bytes.size();
        if (numBytes == 1) {
            codepoint = bytes[0];
        } else if (numBytes == 2) {
            codepoint = ((bytes[0] & 0x1F) << 6) | (bytes[1] & 0x3F);
        } else if (numBytes == 3) {
            codepoint = ((bytes[0] & 0x0F) << 12) | ((bytes[1] & 0x3F) << 6) | (bytes[2] & 0x3F);
        } else if (numBytes == 4) {
            codepoint = ((bytes[0] & 0x07) << 18) | ((bytes[1] & 0x3F) << 12) | ((bytes[2] & 0x3F) << 6) | (bytes[3] & 0x3F);
        }
        return codepoint;
    }

    uint32_t utf8ToLower(uint32_t utf8){
        if ((utf8 >= 'A') && (utf8 <= 'Z')) return (utf8 | 0x20);
        if ((utf8 >= 0xC380) && (utf8 <= 0xC39A) && (utf8 != 0xC397)) return (utf8 | 0x20);

        return utf8;
    }

    void Utf8Parser::readFile(std::filesystem::path filePath) {
            std::ifstream file(filePath, std::ios::binary);
            if (!file) {
                std::cerr << "Failed to open file: " << filePath << std::endl;
                return;
            }

            std::vector<char> buffer(bufferSize * 1024);
            std::string leftover;
            while (file.read(buffer.data(), buffer.size()) || file.gcount() > 0) {
                std::string chunk = leftover + std::string(buffer.data(), file.gcount());
                leftover.clear();

                size_t i = 0;
                while (i < chunk.size()) {
                    size_t start = i;
                    size_t len = getUtf8CharLength(chunk[i]);
                    
                    if (i + len > chunk.size()) {
                        leftover = chunk.substr(start);
                        break;
                    }

                    std::string utf8Char = chunk.substr(start, len);
                    uint32_t codepoint = utf8ToCodepoint(utf8Char);
                    codePoints.insert(utf8ToLower(codepoint));

                    i += len;
                }
            }

            file.close();
        }
}