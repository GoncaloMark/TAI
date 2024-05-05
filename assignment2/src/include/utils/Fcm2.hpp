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

        static void updateFCMCount(const std::string &text, size_t k, FCMCount &fcmCount);
        static FCMCount generateFCMCount(const std::string &text, size_t k);
        static FCMFreq
        generateFCMFreq(FCMCount &fcmCount, double alpha, const std::unordered_set<char> &alphabet);
        static double estimateProbability(double alpha, size_t alphabetSize, char symbol,
                                          std::unordered_map<char, unsigned int> &freqDist);
        static double calculateTextEntropy(std::string &text, size_t alphabetSize, size_t k, double alpha,
                                           FCMFreq fcmFreq, FCMCount fcmCount);
        static void printFCMCount(const FCMCount &fcmCount);
        static void printFCMFreq(const FCMFreq& fcmFreq);

        void update(const std::string& text) {
            updateFCMCount(text, kSize, fcmCount);
        }

        void process() {
            std::cout << "size contexts: " << fcmCount.size() << std::endl;
            fcmFreq = generateFCMFreq(fcmCount, alpha, alphabet);
            std::cout << "size contexts: " << fcmFreq.size() << std::endl;
        }

        void evaluateTestText(std::string& text) {
            auto alphabetSize = static_cast<unsigned int>(alphabet.size());
            double maxEntropy = log2(alphabetSize);
            double testEntropy = calculateTextEntropy(text, alphabetSize, kSize, alpha, fcmFreq,
                                                      fcmCount);
            double nrc = (maxEntropy - testEntropy) / maxEntropy;

            std::cout << "Test Text Entropy: " << testEntropy << std::endl;
            std::cout << "Normalized Relative Compression (NRC): " << nrc << std::endl;
        }





    private:
        size_t kSize;
        double alpha;
        const std::unordered_set<char>& alphabet;

        FCMCount fcmCount;
        FCMFreq fcmFreq;
    };

} // FCM
