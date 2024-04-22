#include <fstream>
#include <iostream>
#include <cmath>
#include "include/fcm.hpp"

namespace FCM {
    void Model::process(){
        std::ifstream file(fileSource);
        if (!file.is_open()) {
            std::cerr << "File is not open or accessible" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::string context;
        char nextChar;
        if (file.get(nextChar)) {
            context.push_back(nextChar);
        }

        while (context.size() < static_cast<size_t>(kSize) && file.get(nextChar)) {
            context.push_back(nextChar);
        }

        while (file.get(nextChar)) {
            if (context.size() == static_cast<size_t>(kSize)) {
                if (counting.find(context) == counting.end()) {
                    counting[context] = std::vector<int>(alphabet.size(), 0);
                }

                if (alphabet.find(nextChar) != alphabet.end()) {
                    int index = charToIndex[nextChar];
                    counting[context][index]++;
                }
            }

            context.erase(0, 1);
            context.push_back(nextChar);
        }

        // printCounting();

        calculateProbabilities();
        // calculateEntropyAndNRC();

        file.close();
    }

    void Model::calculateProbabilities() {
        for (const auto& pair : counting) {
            const std::string& context = pair.first;
            const std::vector<int>& counts = pair.second;
            std::vector<double>& contextProbabilities = probabilities[context];
            
            contextProbabilities.resize(counts.size(), 0.0);

            double denominator = 0.0;
            for (int count : counts) {
                denominator += count + alpha;
            }
            denominator += alpha * (alphabet.size() - counts.size()); 

            for (size_t i = 0; i < counts.size(); ++i) {
                contextProbabilities[i] = (counts[i] + alpha) / denominator;
            }
        }
    }

    double Model::calculateEntropyForText(const std::string& text) {
        double totalEntropy = 0.0;
        int validContexts = 0;
        char nextChar;

        std::string context = text.substr(0, kSize);

        for (size_t i = kSize; i < text.length(); ++i) {
            if (probabilities.find(context) != probabilities.end()) {
                const std::vector<double>& probs = probabilities[context];
                char nextChar = text[i];
                int index = charToIndex[nextChar];

                if (static_cast<size_t>(index) < probs.size() && probs[index] > 0) {
                    totalEntropy += -log2(probs[index]);
                    validContexts++;
                }
            }

            context.erase(0, 1);
            context.push_back(nextChar);
        }

        if (validContexts > 0)
            return totalEntropy / validContexts;  // Average entropy per character
        return 0;
    }

    void Model::evaluateTestText(const std::string& testText) {
        double testEntropy = calculateEntropyForText(testText);
        double maxEntropy = log2(alphabet.size());
        double nrc = (maxEntropy - testEntropy) / maxEntropy;

        std::cout << "Test Text Entropy: " << testEntropy << std::endl;
        std::cout << "Normalized Relative Compression (NRC): " << nrc << std::endl;
    }
}