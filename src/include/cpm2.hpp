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
#include "utils/utils2.hpp"

namespace CPM {
    class CopyModel2 {

    public:
        CopyModel2(std::filesystem::path inputFilePath, double alpha, double threshold, int k) :
                inputFilePath(std::move(inputFilePath)),
                alpha(alpha),
                threshold(threshold),
                kmerSize(k) {};

        void process() {
            reader = UTF8::Utf8Reader(inputFilePath);
            buffer = cbuffer::CircularBuffer<UTF8::Utf8Character>(kmerSize);
            finalHitProbability = 0.0;
            nHits = nFails = count = 0;

            initialise();

            while(true) {
                symbol = reader.getNextCharacter();
                if(reader.isEndOfFile()) {
                    break;
                }

                if(!positions[kmer].empty())
                {
                    if(symbol.getCharacter() == predict()) {
                        nHits++;
                    } else {
                        nFails++;
                    }
                }
                finalHitProbability += UTILS::calculateHitProbability(nHits, nFails, alpha);
                if(isPrecise()) {
                    resetModel();
                }

                updatePositions();

                count++;
                buffer.update(symbol);
                kmer = UTF8::Utf8Character::convertToStr(buffer.toList());
            }
            reader.closeFile();

            std::cout << "End: " << finalHitProbability/count << std::endl;
        };

    private:
        std::filesystem::path inputFilePath;
        double alpha;
        double threshold;
        int kmerSize;

        cbuffer::CircularBuffer<UTF8::Utf8Character> buffer;
        std::string kmer;
        UTF8::Utf8Character symbol;
        uint32_t nHits;
        uint32_t nFails;
        uint32_t count;

        std::unordered_map<std::string, std::vector<uint32_t>> positions;
        double finalHitProbability;
        size_t totalBits;

        UTF8::Utf8Reader reader;

        void initialise() {
            /// -> Get first Kmer
            symbol = reader.getFirstChar();
            for(int i=0;i<kmerSize;i++){
                buffer.enqueue(symbol);
            }
            kmer = UTF8::Utf8Character::convertToStr(buffer.toList());
            /// -> Set file position(optional)
            reader.openFile();
        };

        std::string predict() {
            std::vector<uint32_t> kmerPositions = positions[kmer];
            uint32_t chosenPos = kmerPositions.at(0);
            std::string predictedSym = reader.getCharAt(chosenPos).getCharacter();
            return predictedSym;
        };

        /// @brief This private method tests the precision of our copy model
        /// @return True if model is being precise, False if model is being imprecise over threshold.
        bool isPrecise() const {
            return ( ((nHits + alpha)/(nHits + nFails + 2*alpha)) * 100) < threshold;
        };

        void resetModel() {
            nHits = 0;
            nFails = 0;
        };

        void updatePositions() {
            positions[kmer].push_back(symbol.getPosition());
        };



    };
}
