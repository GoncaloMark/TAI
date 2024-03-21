#pragma once

#include <stdlib.h>
#include <unordered_set>
#include <unordered_map>
#include <cstdint>
#include <algorithm>
#include <array>
#include <tuple>
#include "utils/parser.hpp"
#include "utils/utf8Parser.hpp"
#include "../include/utils/CircularBuffer.hpp"

namespace CPM {
    class CopyModel {
    public:
        explicit CopyModel(const double alpha, const double threshold, const int k, const int bufSize, Parser& decoder) : alpha(alpha), threshold(threshold), kmerSize(k), bufSize(bufSize), decoder(decoder){};

        void start();

    private:
        std::unordered_map<std::string, std::tuple<size_t, uint32_t>> positions;

        std::array<std::vector<uint32_t>, 2> buffers;

        uint32_t Nh;
        uint32_t Nf;

        const double alpha;
        const double threshold;

        size_t totalBits;
        const int kmerSize;

        const int bufSize;

        Parser& decoder;

        /// @brief This private method tests the precision of our copy model
        /// @return True if model is being precise, False if model is being imprecise over threshold.
        bool precision() const {
            return ((Nh + alpha)/(Nh + Nf + 2*alpha)) * 100 < threshold;
        };

        void processBufferTransition(cbuffer::CircularBuffer<uint32_t>& kmerBuf, const std::vector<uint32_t>& buffer1, const std::vector<uint32_t>& buffer2, size_t kmerSize);

        void processBuffer(cbuffer::CircularBuffer<uint32_t>& kmerBuf, const std::vector<uint32_t>& buffer, size_t kmerSize);

        void processKmer(std::string& kmer, size_t pos, size_t bufIndex);

        std::string convertKmerToString(cbuffer::CircularBuffer<uint32_t>& kmerBuf) {
            std::string kmerString;
            auto kmerList = kmerBuf.toList();
            for (auto kmer : kmerList) {
                kmerString.append(decoder.encode(kmer));
            }
            return kmerString;
        }

        void resetModel() {
            Nh = 0;
            Nf = 0;
            positions.clear();
            // buffers[0].clear();
            // buffers[1].clear();
        };
    };
}
