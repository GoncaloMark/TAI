#pragma once

#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <unordered_set>
#include <cmath>
#include "Sparse"
#include "Dense"


namespace FCM {

    typedef std::unordered_map<std::string, std::unordered_map<char, unsigned int>> FCMCount;
    typedef std::unordered_map<std::string, std::unordered_map<char, double>> FCMFreq;

    class Fcm2 {

    public:

        explicit Fcm2(size_t kSize, double alpha, const std::unordered_set<char>& alphabet):
        kSize(kSize), alpha(alpha), alphabet(alphabet) {};

        void update(const std::string& text) {
            std::string context;
            char nextSymbol;

            for (size_t i = 0; i < text.size() - kSize; ++i) {
                // Get the context and the next symbol
                context = text.substr(i, kSize);
                nextSymbol = text[i + kSize];
                fcmCount[context][nextSymbol]++;
            }
        }

        void process() {
            const double constantTerm = static_cast<unsigned int>(alphabet.size()) * alpha;
            double denominator;

            for (auto& contextFreqDistPair : fcmCount) {
                const auto& context = contextFreqDistPair.first;
                auto& counters = contextFreqDistPair.second;

                // get denominator
                denominator = constantTerm;
                for(const auto& counter: counters) {
                    denominator += counter.second;
                }

                for(auto& symbol: alphabet) {
                    fcmFreq[context][symbol] = (counters[symbol] + alpha)/denominator;
                }
            }
            std::cout << "size contexts: " << fcmCount.size() << std::endl;
        }

        // Function to build FCM fcmCount
        void getFCMCount(const std::filesystem::path& filePath) {

            std::ifstream file(filePath);

            if (!file.is_open()) {
                std::cerr << "File is not open or accessible" << std::endl;
                exit(EXIT_FAILURE);
            }

            std::string context;
            char nextChar;

            // Initiate first context
            while(context.size() < kSize && file.get(nextChar)) {
                context.push_back(nextChar);
            }

            while(file.get(nextChar)) {
                fcmCount[context][nextChar]++;

                // update context
                context.erase(0, 1);
                context.push_back(nextChar);
            }

        }



        double calculateEntropy(std::string &text) {
            double totalEntropy = 0.0;
            int validContexts = 0;

            std::string context;
            char nextSymbol;

            for (size_t i = 0; i < text.size() - kSize; ++i) {
                // Get the context and the next symbol
                context = text.substr(i, kSize);
                nextSymbol = text[i + kSize];

                if(fcmFreq.find(context) != fcmFreq.end()) {
                    auto freqs = fcmFreq[context];
                    if(freqs[nextSymbol] > 0) {
                        totalEntropy += -log2(freqs[nextSymbol]);
                        validContexts++;
                    }
                }
            }
            if(validContexts > 0) {
                return totalEntropy / validContexts;
            }
            return 0;
        }

        void evaluateTestText(std::string& text) {
            auto alphabetSize = static_cast<unsigned int>(alphabet.size());
            double maxEntropy = log2(alphabetSize);
            double testEntropy = calculateEntropy(text);
            double nrc = (maxEntropy - testEntropy) / maxEntropy;

            std::cout << "Test Text Entropy: " << testEntropy << std::endl;
            std::cout << "Normalized Relative Compression (NRC): " << nrc << std::endl;
        }

        void printFCMCount();

        static void printFCMFreq(const FCMFreq& fcmProb) {
            // Print the FCM fcmFreq
            for (const auto& contextFreqDistPair : fcmProb) {
                const auto& context = contextFreqDistPair.first;

                std::cout << "Context: " << context << std::endl;
                for (const auto& symbolCountPair : contextFreqDistPair.second) {
                    auto symbol = symbolCountPair.first;
                    auto count = symbolCountPair.second;
                    std::cout << "  Symbol: " << symbol << ", Frequency: " << count << std::endl;
                }
            }
        }

        Eigen::SparseMatrix<double> getFeatureMatrix(FCMCount& fcmCounter) {
            int nContexts, nSymbols, row, col;
            // Get the number of contexts and symbols
            nContexts = static_cast<int>(fcmCounter.size());
            nSymbols = static_cast<int>(alphabet.size());

            // Create a SparseMatrix for the feature matrix
            Eigen::SparseMatrix<double> featureMatrix(nContexts, nSymbols + 1); // +1 for context identifier column

            // Iterate over each context in the FCM model
            row = 0;
            for (const auto& contextFreqDistPair : fcmCounter) {
                const auto& context = contextFreqDistPair.first;
                const auto& freqDist = contextFreqDistPair.second;

                // Store the context identifier in the first column
                featureMatrix.insert(row, 0) = static_cast<double>(hashContext(context));

                for(const auto symbol: alphabet) { // Iterate over symbol in alphabet
                    auto symbolCountPair = freqDist.find(symbol);
                    col = static_cast<int>(static_cast<unsigned char>(symbol)) + 1;
                    double value = 0;
                    if(symbolCountPair != freqDist.end()) {
                        value = symbolCountPair->second;
                    }
                    featureMatrix.insert(row, col) = value;
                }
                row++; // Move to the next row
            }

            featureMatrix.makeCompressed(); // Compress the sparse matrix
            return featureMatrix;
        }

        static size_t hashContext(const std::string& context) {

            size_t hash = 0;
            for (char c : context) {
                hash = (hash * PRIME_NUMBER) + static_cast<size_t>(c);
            }
            return hash;
        }

    private:
        const static size_t PRIME_NUMBER = 31;

        size_t kSize;
        double alpha;
        const std::unordered_set<char>& alphabet;

        FCMCount fcmCount;
        FCMFreq fcmFreq;
        Eigen::SparseMatrix<double> featureMatrix;
    };

} // FCM
