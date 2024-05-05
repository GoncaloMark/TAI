#include "Fcm2.hpp"
#include "helpers.hpp"

namespace FCM {

    void Fcm2::printFCMCount(const FCMCount &fcmCount) {
        // Print the FCM fcmCount
        for (const auto& contextFreqDistPair : fcmCount) {
            const std::string& context = UTILS::escapeString(contextFreqDistPair.first);

            std::cout << "Context: " << context << std::endl;
            for (const auto& symbolCountPair : contextFreqDistPair.second) {
                char symbol = symbolCountPair.first;
                auto count = symbolCountPair.second;
                std::cout << "  Symbol: " << symbol << ", Count: " << count << std::endl;
            }
        }
    }

    void Fcm2::updateFCMCount(const std::string &text, size_t k, FCMCount &fcmCount) {
        std::string context;
        char nextSymbol;

        for (size_t i = 0; i < text.size() - k; ++i) {
            // Get the context and the next symbol
            context = text.substr(i, k);
            nextSymbol = text[i + k];
            fcmCount[context][nextSymbol]++;
        }

    }

    FCMCount Fcm2::generateFCMCount(const std::string &text, size_t k) {
        FCMCount fcmCount;
        updateFCMCount(text, k, fcmCount);
        return fcmCount;
    }

    FCMFreq Fcm2::generateFCMFreq(FCMCount &fcmCount, double alpha, const std::unordered_set<char> &alphabet) {
        const double constTerm = static_cast<unsigned int>(alphabet.size()) * alpha;

        FCMFreq fcmFreq;
        double denominator;

        for (auto& contextFreqDistPair : fcmCount) {
            const auto& context = contextFreqDistPair.first;
            auto& counters = contextFreqDistPair.second;

            // get denominator
            denominator = constTerm;
            for(const auto& counter: counters) {
                denominator += counter.second;
            }

            for(const auto& counter: counters) {
                auto symbol = counter.first;
                auto count = counter.second;
                fcmFreq[context][symbol] = (count + alpha)/denominator;
            }

            /*for(const auto symbol: alphabet) {
                fcmFreq[context][symbol] = (counters[symbol] + alpha)/denominator;
            }*/
        }

        return fcmFreq;
    }

    void Fcm2::printFCMFreq(const FCMFreq &fcmFreq) {
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

    double Fcm2::estimateProbability(double alpha, size_t alphabetSize, char symbol,
                              std::unordered_map<char, unsigned int> &freqDist) {
        const double constTerm = static_cast<unsigned int>(alphabetSize) * alpha;

        double denominator = constTerm;
        for(const auto& counter: freqDist) {
            denominator += counter.second;
        }

        unsigned int count;
        if(freqDist.find(symbol) != freqDist.end()) {
            count = freqDist[symbol];
        } else {
            count = 0;
        }

        return (count + alpha)/denominator;
    }

    double Fcm2::calculateTextEntropy(std::string &text, size_t alphabetSize, size_t k, double alpha,
                                      FCMFreq fcmFreq, FCMCount fcmCount) {
        double totalEntropy = 0.0;
        int validContexts = 0;

        std::string context;
        char nextSymbol;
        double symEntropy;

        for (size_t i = 0; i < text.size() - k; ++i) {
            // Get the context and the next symbol
            context = text.substr(i, k);
            nextSymbol = text[i + k];

            if(fcmFreq.find(context) != fcmFreq.end()) { // if context exists in FCM model
                auto freqDist = fcmFreq[context];
                if(freqDist.find(nextSymbol) != freqDist.end()) { // if nextSymbol is counted in the context of the FCM Model
                    symEntropy = freqDist[nextSymbol];
                } else {
                    symEntropy = estimateProbability(alpha, alphabetSize, nextSymbol, fcmCount[context]);
                }
                totalEntropy += -log2(symEntropy);
                validContexts++;
            }
        }
        if(validContexts > 0) {
            return totalEntropy / validContexts;
        } else {
            return 0;
        }
    }
}