#include "utf8Parser.hpp"

namespace UTF8 {
    uint32_t Utf8Parser::toHex(const std::string& character) {
        std::vector<unsigned char> bytes(character.begin(), character.end());
        uint32_t hex = 0;
        size_t numBytes = bytes.size();
        for (size_t i = 0; i < numBytes; ++i) {
            if (i >= 4) break;

            hex = (hex << 8) | (static_cast<uint8_t>(character[i]) & 0xFF);
        }
        return hex;
    }

    void Utf8Parser::readAll(std::function<void(uint32_t)> callback) {
        if (!file || !file.is_open()) {
            std::cerr << "File is not open or accessible" << std::endl;
            exit(-1);
        }

        file.seekg(0, std::ios::beg);
        std::vector<char> buffer(bufferSize * 1024);
        std::string leftover;

        while (file.read(buffer.data(), buffer.size()) || file.gcount() > 0) {
            std::string chunk = leftover + std::string(buffer.data(), file.gcount());
            leftover.clear();
            size_t i = 0;

            while (i < chunk.size()) {
                size_t start = i;
                size_t len = getCharLength(chunk[i]);
                
                if (i + len > chunk.size()) {
                    leftover = chunk.substr(start);
                    break;
                }
                std::string utf8Char = chunk.substr(start, len);
                uint32_t hex = toHex(utf8Char);
                callback(hex);
                i += len;
            }
        }

        file.clear();
        file.seekg(0, std::ios::beg);
    }

        bool Utf8Parser::readChunk(std::vector<std::vector<uint32_t>>& buffers, size_t bufferSize) {
            if (!file || !file.is_open()) {
                std::cerr << "File is not open or accessible" << std::endl;
                exit(-1);
            }

            //TODO Clear buffers... in a correct manner
            buffers[0].clear();
            buffers[1].clear();

            // Seek to the last known position
            file.seekg(readPosition);

            size_t totalBytesToRead = bufferSize * buffers.size();
            std::vector<char> charBuffer(totalBytesToRead);
            std::cout << "TOTAL BYTES: " << totalBytesToRead << std::endl;
            std::cout << "Position: " << readPosition << std::endl;
            std::string leftover;

            if (file.read(charBuffer.data(), charBuffer.size()) || file.gcount() > 0) {
                // Update readPosition with the new position
                readPosition = file.tellg();

                std::string chunk = leftover + std::string(charBuffer.data(), file.gcount());
                leftover.clear();

                size_t i = 0, bufferIndex = 0;
                while (i < chunk.size() && bufferIndex < buffers.size()) {
                    size_t start = i;
                    size_t len = getCharLength(chunk[i]);

                    if (i + len > chunk.size()) {
                        leftover = chunk.substr(start);
                        break;
                    }

                    if (buffers[bufferIndex].size() * sizeof(uint32_t) >= bufferSize) {
                        bufferIndex++;
                        if (bufferIndex >= buffers.size()) break;
                    }

                    std::string utf8Char = chunk.substr(start, len);
                    uint32_t hex = toHex(utf8Char);
                    buffers[bufferIndex].push_back(hex);

                    i += len;
                }
                return true;
            }
            return false;
        }

        std::string Utf8Parser::encode(uint32_t character){
            std::stringstream ss;
            if (character & 0xFF000000) {  // Most significant byte (leftmost)
                ss << static_cast<char>((character & 0xFF000000) >> 24);
            }
            if (character & 0x00FF0000) {
                ss << static_cast<char>((character & 0x00FF0000) >> 16);
            }
            if (character & 0x0000FF00) {
                ss << static_cast<char>((character & 0x0000FF00) >> 8);
            }
            if (character & 0x000000FF) {  // Least significant byte (rightmost)
                ss << static_cast<char>(character & 0x000000FF);
            }
            return ss.str();
        }
}