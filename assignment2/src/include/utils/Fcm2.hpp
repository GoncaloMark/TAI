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
         * otherwise it will get delete as the build method gets deleted.
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
        buildModel(size_t k, double alpha, const std::unordered_set<char> &alphabet, std::vector<std::string> &texts);

        void evaluateTestText(std::string& text) {
            auto alphabetSize = static_cast<unsigned int>(alphabet.size());
            double maxEntropy = log2(alphabetSize);
            double testEntropy = calculateTextEntropy(text, alphabetSize, kSize, alpha, fcmFreq, contCounter);
            double nrc = (maxEntropy - testEntropy) / maxEntropy;

            std::cout << "Test Text Entropy: " << testEntropy << std::endl;
            std::cout << "Normalized Relative Compression (NRC): " << nrc << std::endl;
        }

    };

} // FCM
