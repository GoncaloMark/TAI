#pragma once

#include <stdlib.h>
#include <unordered_set>
#include <unordered_map>
#include <cstdint>
#include "utils/parser.hpp"
#include "utils/utf8Parser.hpp"

namespace CPM {
    class CopyModel {
    public:
        explicit CopyModel(std::filesystem::path inputFilePath, const double alpha, const double threshold, const int k, Parser& decoder) : inputFilePath(std::move(inputFilePath)), alpha(alpha), threshold(threshold), kmerSize(k), decoder(decoder){};

        void start();

        //TODO USE DOUBLE BUFFER IMPLEMENTATION.
    private:
        std::filesystem::path inputFilePath;
        std::unordered_map<std::string, uint32_t> positions;

        std::vector<uint32_t> buffer1;
        std::vector<uint32_t> buffer2;

        uint32_t Nh;
        uint32_t Nf;

        const double alpha;
        const double threshold;

        size_t totalBits;
        const int kmerSize;

        //TODO Refactor decoder to get byte sequence of length K.
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

        /// @brief This private method tests the precision of our copy model
        /// @return True if model is being precise, False if model is being imprecise over threshold.
        bool precision() const {
            return ((Nh / (Nh + Nf)) * 100) < threshold;
        };

        void predict(uint32_t symbol) const;
    };
}
