#include "../include/cpm.hpp"

namespace CPM {
    void CopyModel::start(){
        bool flag;
        Nh = Nf = 0;
        
        // double HitProbability = 0.0;
        cbuffer::CircularBuffer<uint32_t> kmerBuf(kmerSize);

        while (true) {
            flag = decoder.readChunk(buffer1, bufSize);
            if (!flag) {
                std::cerr << "Error reading chunk or end of data...\n";
                break;
            }

            if (!buffer2.empty()) {
                processBufferTransition(kmerBuf, buffer2, buffer1, kmerSize);
            }

            processBuffer(kmerBuf, buffer1, kmerSize);

            buffer2 = buffer1;
        }
    }

    void CopyModel::processBuffer(cbuffer::CircularBuffer<uint32_t>& kmerBuf, const std::vector<uint32_t>& buffer, size_t kmerSize) {
            // Logic to process the current buffer
            for (size_t index = 0; index <= buffer.size() - kmerSize; ++index) {
                kmerBuf.clear();
                for (size_t i = 0; i < kmerSize; ++i) {
                    kmerBuf.enqueue(buffer[index + i]);
                }
                std::string kmerString = convertKmerToString(kmerBuf);
                std::cout << kmerString << std::endl;
            }
        }

    void CopyModel::processBufferTransition(cbuffer::CircularBuffer<uint32_t>& kmerBuf, const std::vector<uint32_t>& buffer2, const std::vector<uint32_t>& buffer1, size_t kmerSize) {
            if (buffer2.size() >= kmerSize - 1) {
                for (size_t i = buffer2.size() - kmerSize + 1; i < buffer2.size(); ++i) {
                    kmerBuf.clear();

                    // Start from the end of buffer2
                    for (size_t j = i; j < buffer2.size(); ++j) {
                        kmerBuf.enqueue(buffer2[j]);
                    }

                    // Fill the rest from the start of buffer1
                    for (size_t j = 0; kmerBuf.size() < kmerSize && j < buffer1.size(); ++j) {
                        kmerBuf.enqueue(buffer1[j]);
                    }

                    if (kmerBuf.size() == kmerSize) {
                        std::string kmerString = convertKmerToString(kmerBuf);
                        std::cout << kmerString << std::endl;
                    }
                }
            }
        }
}

