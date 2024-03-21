#pragma once

#include <stdlib.h>
#include <unordered_set>
#include <unordered_map>
#include <cstdint>
#include <algorithm>
#include "utils/parser.hpp"
#include "utils/utf8Parser.hpp"
#include "../include/utils/CircularBuffer.hpp"

namespace CPM {
    class CopyModel {
    public:
        explicit CopyModel(const double alpha, const double threshold, const int k, const int bufSize, Parser& decoder) : alpha(alpha), threshold(threshold), kmerSize(k), bufSize(bufSize), decoder(decoder){};

        void start();

    private:
        std::unordered_map<std::string, uint32_t> positions;

        std::vector<uint32_t> buffer1;
        std::vector<uint32_t> buffer2;

        uint32_t Nh;
        uint32_t Nf;

        const double alpha;
        const double threshold;

        size_t totalBits;
        const int kmerSize;

        const int bufSize;

        Parser& decoder;

        /// @brief Private method for finding kmer in the hashtable.
        /// @param kmer String reference for a given kmer.
        /// @return Position in buffer.
        uint32_t getKmer(std::string& kmer) const {
            uint32_t pos;
            try{
                pos = positions.at(kmer);
            } catch(std::exception& e){
                pos = 0;
            }

            return pos;
        };

        std::string convertKmerToString(cbuffer::CircularBuffer<uint32_t>& kmerBuf) {
            std::string kmerString;
            auto kmerList = kmerBuf.toList();
            for (auto kmer : kmerList) {
                kmerString.append(decoder.encode(kmer));
            }
            return kmerString;
        }

        /// @brief This private method tests the precision of our copy model
        /// @return True if model is being precise, False if model is being imprecise over threshold.
        bool precision() const {
            return ((Nh / (Nh + Nf)) * 100) < threshold;
        };

        void processBufferTransition(cbuffer::CircularBuffer<uint32_t>& kmerBuf, const std::vector<uint32_t>& buffer2, const std::vector<uint32_t>& buffer1, size_t kmerSize);

        void processBuffer(cbuffer::CircularBuffer<uint32_t>& kmerBuf, const std::vector<uint32_t>& buffer, size_t kmerSize);

        void predict(uint32_t symbol) const;
    };
}
