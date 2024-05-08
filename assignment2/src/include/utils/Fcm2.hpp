#pragma once

#include <iostream>
#include <unordered_map>
#include <map>
#include <utility>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <unordered_set>
#include <cmath>
#include <chrono>
#include "csv.hpp"
#include "helpers.hpp"



namespace FCM {

    typedef std::unordered_map<std::string, unsigned long> ContextCounter;
    typedef std::unordered_map<std::string, std::unordered_map<char, unsigned int>> FCMCount;
    typedef std::unordered_map<std::string, std::unordered_map<char, double>> FCMFreq;

    class FCMModel {

    private:
        size_t kSize;
        double alpha;
        const std::unordered_set<char>& alphabet;
        /*
         * Since we are creating it from the build method, we cannot pass as reference,
         * otherwise it will get delete as soon as the build method finishes.
         * */
        //
        const ContextCounter contCounter;
        const FCMFreq fcmFreq;

        FCMModel(size_t kSize, double alpha,
                          const std::unordered_set<char>& alphabet,
                          const ContextCounter contCounter,
                          const FCMFreq fcmFreq):
                kSize(kSize), alpha(alpha), alphabet(alphabet), contCounter(contCounter), fcmFreq(fcmFreq) {
        }

        static FCMCount generateBaseModel(const std::filesystem::path &dataPath, size_t k);
        static void updateFCMCount(const std::string &text, size_t k, FCMCount &fcmCount);
        static FCMCount generateFCMCount(const std::string &text, size_t k);
        static ContextCounter generateContextCounter(FCMCount &fcmCount);
        static FCMFreq generateFCMFreq(FCMCount &fcmCount, double alpha, const std::unordered_set<char> &alphabet,
                                       ContextCounter &contCounter);
        static FCMFreq generateFinalFCMFreq(FCMCount &fcmCount, double alpha, const std::unordered_set<char> &alphabet,
                                       ContextCounter &contCounter);
        static double estimateProbability(double alpha, size_t alphabetSize, unsigned long contextCount,
                                          unsigned int contextSymCount);
        static void printFCMCount(const FCMCount &fcmCount);
        static void printFCMFreq(const FCMFreq& fcmFreq);
        static size_t fcmFreqMemUsage(const FCMFreq& fcmFreq, size_t k);
        static size_t contCounterMemUsage(const ContextCounter& contCounter, size_t k);
        static double calculateTextEntropy(
                std::string &text,
                size_t alphabetSize,
                size_t k,
                double alpha,
                const FCMFreq &fcmFreq,
                const ContextCounter &contCounter);

    public:

        static FCMModel
        buildModel(size_t k, double alpha, const std::unordered_set<char> &alphabet,
                   const std::filesystem::path &dataPath);

        static bool wasRewrittenChatGpt(std::string& text, FCMModel &rhModel, FCMModel &rcModel);

        static void evaluate(const std::filesystem::path &testDataPath, FCMModel &rhModel, FCMModel &rcModel);

        double getTextEntropy(std::string& text) {
            return calculateTextEntropy(text, alphabet.size(), kSize, alpha, fcmFreq, contCounter);
        }

        double getModelMaxEntropy() {
            return log2(static_cast<double>(alphabet.size()));
        }

        double getTextNRC(std::string& text) {
            double maxEntropy = getModelMaxEntropy();
            double textEntropy = getTextEntropy(text);
            return (maxEntropy - textEntropy) / maxEntropy;
        }

        void evaluateText(std::string& text) {
            std::cout << "Text Entropy: " << getTextEntropy(text) << std::endl;
            std::cout << "Normalized Relative Compression (NRC): " << getTextNRC(text) << std::endl;
        }

    };

} // FCM
