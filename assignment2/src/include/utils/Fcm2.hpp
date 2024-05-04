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

namespace FCM {

    typedef std::unordered_map<std::string, std::unordered_map<char, unsigned int>> FCMCount;
    typedef std::unordered_map<std::string, std::unordered_map<char, double>> FCMFreq;

    class Fcm2 {

    public:

        explicit Fcm2(size_t kSize, double alpha, const std::unordered_set<char>& alphabet): kSize(kSize), alpha(alpha),
        alphabet(alphabet) {};

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

        void calculateRelativeFreq() {

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
            double testEntropy = calculateEntropy(text);
            double maxEntropy = log2(alphabet.size());
            double nrc = (maxEntropy - testEntropy) / maxEntropy;

            std::cout << "Test Text Entropy: " << testEntropy << std::endl;
            std::cout << "Normalized Relative Compression (NRC): " << nrc << std::endl;
        }

        void printFCMCount() {
            // Print the FCM fcmCount
            for (const auto& contextFreqDistPair : fcmCount) {
                const std::string& context = contextFreqDistPair.first;

                std::cout << "Context: " << context << std::endl;
                for (const auto& symbolCountPair : contextFreqDistPair.second) {
                    char symbol = symbolCountPair.first;
                    auto count = symbolCountPair.second;
                    std::cout << "  Symbol: " << symbol << ", Count: " << count << std::endl;
                }
            }
        }

        void printFCMFreq() {
            // Print the FCM fcmFreq
            for (const auto& contextFreqDistPair : fcmFreq) {
                const auto& context = contextFreqDistPair.first;

                std::cout << "Context: " << context << std::endl;
                for (const auto& symbolCountPair : contextFreqDistPair.second) {
                    auto symbol = symbolCountPair.first;
                    auto count = symbolCountPair.second;
                    std::cout << "  Symbol: " << symbol << ", Frequency: " << count << std::endl;
                }
            }
        }



        void fcmModelToFeatureMatrix() {
            // Get the number of contexts and symbols
            int numContexts = static_cast<int>(fcmCount.size());
            int numSymbols = static_cast<int>(alphabet.size());

            // Iterate over each text in the FCM model
            for (const auto& contextFreqDistPair : fcmCount) {
                const auto& context = contextFreqDistPair.first;
                const auto& freqDist = contextFreqDistPair.second;

                // Create a SparseMatrix for the current text
                Eigen::SparseMatrix<double> featureMatrix(numContexts, numSymbols);

                // Populate the feature matrix
                for (const auto& symbolCountPair : freqDist) {
                    const auto& symbol = symbolCountPair.first;
                    int row = stoi(context); // Convert context string to integer
                    int col = static_cast<unsigned char>(symbol); // Convert symbol to ASCII value
                    featureMatrix.insert(row, col) = static_cast<double>(symbolCountPair.second);
                }

                // Add the feature matrix to the list of feature matrices
                featureMatrices.push_back(featureMatrix);
            }
        }

        void printFeatureMatrix(int index) {
            std::cout << "Feature Matrix for Text " << index << ":" << std::endl;
            std::cout << featureMatrices[index] << std::endl;
        }

    private:
        size_t kSize;
        double alpha;
        const std::unordered_set<char>& alphabet;

        FCMCount fcmCount;
        FCMFreq fcmFreq;
        std::vector<Eigen::SparseMatrix<double>> featureMatrices;

        Eigen::SparseMatrix<double> featureMatrixTmp;

    };

} // FCM
