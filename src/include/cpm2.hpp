#pragma once

#include <stdlib.h>
#include <unordered_set>
#include <unordered_map>
#include <cstdint>
#include <utility>
#include "utils/parser.hpp"
#include "utils/utf8Parser.hpp"
#include "utils/CircularBuffer.hpp"
#include "utils/Utf8Reader.hpp"

namespace CPM {
    class CopyModel2 {

        //TODO USE DOUBLE BUFFER IMPLEMENTATION.
    private:
        std::filesystem::path inputFilePath;
        int kmerSize;
        double alpha;
        double threshold;

        cbuffer::CircularBuffer<UTF8::Utf8Character> buffer;
        std::string curKmer;
        UTF8::Utf8Character curSymbol;
        uint32_t nHits;
        uint32_t nFails;

        std::unordered_map<std::string, uint32_t> positions;
        std::unordered_map<std::string, std::vector<uint32_t>> nextCharPositions;
        double finalHitProbability;
        size_t totalBits;

        UTF8::Utf8Reader reader;
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
            return ((nHits / (nHits + nFails)) * 100) < threshold;
        };

        void predict(uint32_t symbol) const;

        bool prediction(std::string kmer, UTF8::Utf8Character symbol) {
            return false;
        }

        void initialise() {
            /// Initialise
            finalHitProbability = 0.0;
            nHits = nHits = 0;
            /// -> Get first Kmer
            /// -> Set file position(optional)
            curSymbol = reader.getFirstChar();
            for(int i=0;i<kmerSize;i++){
                buffer.enqueue(curSymbol);
            }
            curKmer = UTF8::Utf8Character::convertToStr(buffer.toList());
            reader.openFile();
        };

    public:
        CopyModel2(std::filesystem::path inputFilePath, double alpha, double threshold, int k, Parser& decoder) :
                inputFilePath(std::move(inputFilePath)),
                alpha(alpha),
                threshold(threshold),
                kmerSize(k),
                decoder(decoder) {
            this->reader = UTF8::Utf8Reader(this->inputFilePath);
            this->buffer = cbuffer::CircularBuffer<UTF8::Utf8Character>(this->kmerSize);
        };

        void startCompression();

        void process() {
            initialise();

            while(!reader.isEndOfFile()) {
                curSymbol = reader.getNextCharacter();
                if(reader.isEndOfFile()) {
                    break;
                }
                buffer.update(curSymbol);
                curKmer = UTF8::Utf8Character::convertToStr(buffer.toList());
            }

            reader.closeFile();
        };

    };
}
