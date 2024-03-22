#include "../include/cpm.hpp"
#include "../include/utils/utils.hpp"

namespace CPM {
    void CopyModel::start(){
        bool flag;
        Nh = Nf = 0;
        size_t count = 0;
        
        double HitProbability = 0.0;
        cbuffer::CircularBuffer<uint32_t> kmerBuf(kmerSize);

        while (true) {
            flag = decoder.readChunk(buffers[0], bufSize);
            if (!flag) {
                std::cerr << "Error reading chunk or end of data...\n";
                break;
            }

            if (!buffers[1].empty()) {
                processBufferTransition(kmerBuf, buffers[0], buffers[1], kmerSize);
            }

            processBuffer(kmerBuf, buffers[0], kmerSize);

            buffers[1] = buffers[0];
            updateBufferIndices();
            count++;
        }
        HitProbability = calculateHitProbability(Nh, Nf, alpha);
        std::cout << "End: " << HitProbability/count << std::endl;
    }

    void CopyModel::updateBufferIndices() {
        std::unordered_map<std::string, std::tuple<size_t, uint32_t>> updatedPositions;

        for (auto& position : positions) {
            const auto& kmer = position.first;
            auto& [bufIndex, pos] = position.second;

            // If k-mer in buffer2 (index 1), it needs to be deleted
            if (bufIndex == 1) {
                continue; // Skip and delete this k-mer
            }

            // If k-mer in buffer1 (index 0), update its index to 1
            if (bufIndex == 0) {
                // Check if the next position is within bounds
                if (pos + kmerSize < buffers[1].size()) {
                    updatedPositions[kmer] = std::make_tuple(1, pos);
                }
            }
        }

        // Replace the old map
        positions = std::move(updatedPositions);
    }

    //TODO Pos is end position of kmer not start of kmer
    void CopyModel::processKmer(std::string& kmer, size_t pos, size_t bufIndex){
        if (positions.find(kmer) != positions.end()) {
            size_t prevBufIndex = std::get<0>(positions[kmer]);
            uint32_t prevPos = std::get<1>(positions[kmer]);

            bool match = false;
            if (prevBufIndex == bufIndex && pos + kmerSize < buffers[bufIndex].size()) {
                match = buffers[bufIndex][prevPos + kmerSize] == buffers[bufIndex][pos + kmerSize];
            } else if (prevBufIndex != bufIndex && prevBufIndex < buffers.size() && prevPos + kmerSize < buffers[prevBufIndex].size() && pos + kmerSize < buffers[bufIndex].size()) {
                match = buffers[prevBufIndex][prevPos + kmerSize] == buffers[bufIndex][pos + kmerSize];
            }

            if (match) {
                Nh++;
            } else {
                Nf++;
            }

            // Update the position
            positions[kmer] = std::make_tuple(bufIndex, pos);
        } else {
            positions.insert({kmer, std::make_tuple(bufIndex, pos)});
        }

        if (!precision()) {
            resetModel();
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
            // std::cout << kmerString << std::endl;

            processKmer(kmerString, index, 0);
        }
    }

    void CopyModel::processBufferTransition(cbuffer::CircularBuffer<uint32_t>& kmerBuf, const std::vector<uint32_t>& buffer1, const std::vector<uint32_t>& buffer2, size_t kmerSize) {
        if (buffer2.size() >= kmerSize - 1) {
            for (size_t i = buffer2.size() - kmerSize + 1; i < buffer2.size(); ++i) {
                kmerBuf.clear();
                size_t buffer1Index = 0; // Track the position in buffer1

                // Start from the end of buffer2
                for (size_t j = i; j < buffer2.size(); ++j) {
                    kmerBuf.enqueue(buffer2[j]);
                }

                // Fill the rest from the start of buffer1
                while (kmerBuf.size() < kmerSize && buffer1Index < buffer1.size()) {
                    kmerBuf.enqueue(buffer1[buffer1Index]);
                    buffer1Index++;
                }

                if (kmerBuf.size() == kmerSize) {
                    std::string kmerString = convertKmerToString(kmerBuf);
                    // std::cout << kmerString << std::endl;

                    // Here, we're passing buffer1Index - 1 as the position where the k-mer ends in buffer1.
                    processKmer(kmerString, buffer1Index - 1, 1);
                }
            }
        }
    }
}

