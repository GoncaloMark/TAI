#pragma once

#include <stdlib.h>
#include <unordered_set>
#include <unordered_map>
#include <cstdint>
#include <algorithm>
#include <array>
#include <tuple>
#include <math.h>
#include "utils/parser.hpp"
#include "utils/utf8Parser.hpp"
#include "../include/utils/utils.hpp"
#include "../include/utils/CircularBuffer.hpp"

namespace CPM {
    class CopyModel {
    public:
        explicit CopyModel(const double alpha, const double threshold, const int k, const int bufSize, Parser& decoder) : alpha(alpha), threshold(threshold), kmerSize(k), bufSize(bufSize), decoder(decoder){};

        void start();

        double getTotalBits() const {
            return totalBits;
        };

    private:
        std::unordered_map<std::string, std::tuple<size_t, uint32_t>> positions;

        std::array<std::vector<uint32_t>, 2> buffers;

        uint32_t Nh;
        uint32_t Nf;

        const double alpha;
        const double threshold;

        double totalBits;
        const int kmerSize;
        const int bufSize;

        Parser& decoder;

        /// @brief This private method tests the precision of our copy model
        /// @return True if model is being precise, False if model is being imprecise over threshold.
        bool precision() const {
            return ((Nh + alpha)/(Nh + Nf + 2*alpha)) * 100 < threshold;
        };

        ///
        /// \param kmerBuf
        /// \param buffer1
        /// \param buffer2
        /// \param kmerSize
        void processBufferTransition(cbuffer::CircularBuffer<uint32_t>& kmerBuf, const std::vector<uint32_t>& buffer1, const std::vector<uint32_t>& buffer2, size_t kmerSize);

        ///
        /// \param kmerBuf
        /// \param buffer
        /// \param kmerSize
        void processBuffer(cbuffer::CircularBuffer<uint32_t>& kmerBuf, const std::vector<uint32_t>& buffer, size_t kmerSize);


        void processKmer(std::string& kmer, size_t pos, size_t bufIndex);

        /// @brief converts a circular buffer of utf8 characters in hexadecimal encoding in a string
        /// \param kmerBuf circular buffer
        /// \return Kmer in string format
        std::string convertKmerToString(cbuffer::CircularBuffer<uint32_t>& kmerBuf) {
            std::string kmerString;
            auto kmerList = kmerBuf.toList();
            for (auto kmer : kmerList) {
                kmerString.append(decoder.encode(kmer));
            }
            return kmerString;
        }

        /// @brief Resets the model in case it is not precise
        void resetModel() {
            Nh = 0;
            Nf = 0;
            positions.clear();
            // buffers[0].clear();
            // buffers[1].clear();
        };

        ///
        void updateBufferIndices();

        static double getEntropyProbability(double probability);
    };
}
