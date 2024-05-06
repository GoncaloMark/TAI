#include "Fcm2.hpp"

namespace FCM {

    void FCMModel::printFCMCount(const FCMCount &fcmCount) {
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

    void FCMModel::updateFCMCount(const std::string &text, size_t k, FCMCount &fcmCount) {
        std::string context;
        char nextSymbol;

        for (size_t i = 0; i < text.size() - k; ++i) {
            // Get the context and the next symbol
            context = text.substr(i, k);
            nextSymbol = text[i + k];
            fcmCount[context][nextSymbol]++;
        }

    }

    FCMCount FCMModel::generateFCMCount(const std::string &text, size_t k) {
        FCMCount fcmCount;
        updateFCMCount(text, k, fcmCount);
        return fcmCount;
    }

    FCMFreq FCMModel::generateFCMFreq(FCMCount &fcmCount, double alpha, const std::unordered_set<char> &alphabet,
                                      ContextCounter &contCounter) {
        const double constTerm = static_cast<unsigned int>(alphabet.size()) * alpha;

        FCMFreq fcmFreq;
        double denominator;

        for (auto& contextFreqDistPair : fcmCount) {
            const auto& context = contextFreqDistPair.first;
            auto& freqDist = contextFreqDistPair.second;

            // get denominator
            denominator = constTerm + static_cast<double>(contCounter[context]);

            for(const auto& symbolCountPair : freqDist) {
                auto symbol = symbolCountPair.first;
                auto count = symbolCountPair.second;
                fcmFreq[context][symbol] = (count + alpha)/denominator;
            }

            /*for(const auto symbol: alphabet) {
                fcmFreq[context][symbol] = (freqDist[symbol] + alpha)/denominator;
            }*/
        }

        return fcmFreq;
    }

    FCMFreq FCMModel::generateFinalFCMFreq(FCMCount &fcmCount, double alpha, const std::unordered_set<char> &alphabet,
                                           ContextCounter &contCounter) {
        const double constTerm = static_cast<unsigned int>(alphabet.size()) * alpha;

        FCMFreq fcmFreq;
        double denominator;

        for (auto& contextFreqDistPair : fcmCount) {
            const auto& context = contextFreqDistPair.first;
            auto& freqDist = contextFreqDistPair.second;

            // get denominator
            denominator = constTerm + static_cast<double>(contCounter[context]);

            for(const auto symbol: alphabet) {
                fcmFreq[context][symbol] = (freqDist[symbol] + alpha)/denominator;
            }
        }

        return fcmFreq;
    }

    void FCMModel::printFCMFreq(const FCMFreq &fcmFreq) {
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

    double FCMModel::estimateProbability(double alpha, size_t alphabetSize, unsigned long contextCount,
                                         unsigned int contextSymCount) {
        const double constTerm = static_cast<unsigned int>(alphabetSize) * alpha;

        double denominator = (constTerm + static_cast<double>(contextCount));
        return (contextSymCount + alpha)/denominator;
    }

    double FCMModel::calculateTextEntropy(std::string &text, size_t alphabetSize, size_t k, double alpha, const FCMFreq &fcmFreq,
                                          const ContextCounter &contCounter) {
        double totalEntropy = 0.0;
        int validContexts = 0;

        std::string context;
        char nextSymbol;
        double symEntropy;

        for (size_t i = 0; i < text.size() - k; ++i) {
            context = text.substr(i, k);
            nextSymbol = text[i + k];

            if(fcmFreq.find(context) != fcmFreq.end()) { // if context exists in FCM model
                auto freqDist = fcmFreq.at(context);
                if(freqDist.find(nextSymbol) != freqDist.end()) { // if nextSymbol is counted in the context of the FCM Model
                    symEntropy = freqDist[nextSymbol];
                } else {
                    symEntropy = estimateProbability(alpha, alphabetSize, contCounter.at(context), 0);
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

    ContextCounter FCMModel::generateContextCounter(FCMCount &fcmCount) {
        ContextCounter contCounter;
        for (auto& contextFreqDistPair : fcmCount) {
            const auto& context = contextFreqDistPair.first;
            const auto& freqDist = contextFreqDistPair.second;

            contCounter[context] = 0;
            for (const auto& symbolCountPair : freqDist) {
                contCounter[context] += symbolCountPair.second;
            }
        }
        return contCounter;
    }

    FCMModel FCMModel::buildModel(size_t k, double alpha, const std::unordered_set<char> &alphabet,
                                  std::vector<std::string> &texts) {
        auto start = std::chrono::high_resolution_clock::now();
        FCMCount fcmCount;
        for(auto& text: texts) {
            updateFCMCount(text, k, fcmCount);
        }
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start);
        std::cout << "Texts update - Execution time: " << duration.count()/1000000.0 << "s" << std::endl;
        ContextCounter contCounter = generateContextCounter(fcmCount);

        //start = std::chrono::high_resolution_clock::now();

        FCMFreq fcmFreq = generateFCMFreq(fcmCount, alpha, alphabet, contCounter);

        //duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start);
        //size_t model1Size = contCounterMemUsage(contCounter, k) + fcmFreqMemUsage(fcmFreq, k);
        //std::cout << "fcmFreq1 generation - Execution time: " << duration.count()/1000000.0 << "s" << std::endl;
        //start = std::chrono::high_resolution_clock::now();
        //FCMFreq fcmFreq2 = generateFinalFCMFreq(fcmCount, alpha, alphabet, contCounter);
        //duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start);
        //size_t model2Size = fcmFreqMemUsage(fcmFreq2, k);
        //std::cout << "fcmFreq2 generation - Execution time: " << duration.count()/1000000.0 << "s" << std::endl;
        //std::cout << "Model size 1: " << model1Size << std::endl;
        //std::cout << "Model size 2: " << model2Size << std::endl;
        //double ratio = (static_cast<double>(model1Size) / static_cast<double>(model2Size) );
        //std::cout << "ratio: " << ratio * 100.0 << std::endl;

        FCMModel model(k, alpha, alphabet, contCounter, fcmFreq);
        return model;
    }

    size_t FCMModel::fcmFreqMemUsage(const FCMFreq &fcmFreq, size_t k) {
        size_t charSize = sizeof(char);
        size_t doubleSize = sizeof(double);

        size_t totalContextSize = fcmFreq.size() * k * charSize;

        size_t totalSize = totalContextSize;
        for (const auto& ContextFreqDist : fcmFreq) {
            auto& freqDist = ContextFreqDist.second;
            totalSize += freqDist.size() * (charSize + doubleSize);
        }
        return totalSize;
    }

    size_t FCMModel::contCounterMemUsage(const ContextCounter &contCounter, size_t k) {
        size_t charSize = sizeof(char);
        size_t longSize = sizeof(unsigned long);

        size_t totalContextSize = contCounter.size() * k * charSize;
        size_t totalCountSize = contCounter.size() * longSize;

        return totalContextSize + totalCountSize;
    }
}