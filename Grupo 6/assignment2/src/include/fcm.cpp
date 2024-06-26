#include "fcm.hpp"

namespace FCM {

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

    FCMFreq FCMModel::generateFCMFreq(FCMCount &fcmCount, double alpha, const std::unordered_set<char> &alphabet,
                                      ContextCounter &contCounter) {
        size_t alphabetSize = alphabet.size();

        FCMFreq fcmFreq;

        for (auto& contextFreqDistPair : fcmCount) {
            const auto& context = contextFreqDistPair.first;
            auto& freqDist = contextFreqDistPair.second;
            auto contextCount = contCounter[context];

            for(const auto& symbolCountPair : freqDist) {
                auto symbol = symbolCountPair.first;
                auto contextSymCount = symbolCountPair.second;
                fcmFreq[context][symbol] = estimateProbability(alpha, alphabetSize, contextCount, contextSymCount);
            }

        }

        return fcmFreq;
    }

    void FCMModel::printFCMFreq(const FCMFreq &fcmFreq) {
        // Print the FCM fcmFreq
        for (const auto& contextFreqDistPair : fcmFreq) {
            const auto& context = UTILS::escapeString(contextFreqDistPair.first);

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
        return (contextSymCount + alpha)/(constTerm + static_cast<double>(contextCount));
    }

    double FCMModel::calculateTextEntropy(std::string &text, size_t alphabetSize, size_t k, double alpha, const FCMFreq &fcmFreq,
                                          const ContextCounter &contCounter) {
        double totalEntropy = 0.0;
        int validContexts = 0;

        std::string context;
        char nextSymbol;
        double symEntropy;

        for (size_t i = 0; i < text.size() - k; ++i) {
            try {
                context = text.substr(i, k);
                nextSymbol = text[i + k];
            } catch (const std::exception& e) {
                std::cout << text.size() << " - " << k << std::endl;
                std::cerr << "Error: " << e.what() << std::endl;
                throw;
            }
            

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
                                  const std::filesystem::path &dataPath) {
        FCMCount fcmCount = generateBaseModel(dataPath, k);
        ContextCounter contCounter = generateContextCounter(fcmCount);
        FCMFreq fcmFreq = generateFCMFreq(fcmCount, alpha, alphabet, contCounter);
        FCMModel model(k, alpha, alphabet, contCounter, fcmFreq);
        return model;
    }

    bool FCMModel::wasRewrittenChatGpt(std::string &text, FCMModel &rhModel, FCMModel &rcModel) {
        double rhModelNRC = rhModel.getTextNRC(text);
        double rcModelNRC = rcModel.getTextNRC(text);

        if(rcModelNRC < rhModelNRC) {
            return false;
        }
        // rcModelNRC >= rhModelNRC
        // TODO
        return true;
    }

    FCMCount FCMModel::generateBaseModel(const std::filesystem::path &dataPath, size_t k) {
        auto start = std::chrono::high_resolution_clock::now();
        FCMCount fcmCount;
        csv::CSVReader reader(dataPath.string());
        std::string text;
        for (auto& row: reader) {
            text = row[0].get<std::string>();
            if(text.size() <= k) {
                continue;
            }
            updateFCMCount(text, k, fcmCount);
        }
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start);

        auto total_seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
        auto mins = std::chrono::duration_cast<std::chrono::minutes>(total_seconds);
        auto secs = std::chrono::duration_cast<std::chrono::seconds>(total_seconds - mins);
        std::cout << "Model text-feeding execution time: " << mins.count() << "m:" << secs.count() << "s" << std::endl;
        return fcmCount;
    }

    void FCMModel::evaluate(const std::filesystem::path &testDataPath, FCMModel &rhModel, FCMModel &rcModel) {
        unsigned int hits, fails, count;
        hits = fails = count = 0;
        std::string text;
        int label;

        csv::CSVReader reader(testDataPath.string());
        for (auto& row: reader) {
            text = row[0].get<std::string>();
            label = row[1].get<int>();
            if(text.size() <= rhModel.getK() || text.size() <= rcModel.getK()) {
                continue;
            }
            int prediction = static_cast<int>(wasRewrittenChatGpt(text, rhModel, rcModel));
            count++;
            if(label == prediction) {
                hits++;
            } else {
                fails++;
            }
        }

        std::cout << "Count: " << count << std::endl;
        std::cout << "Hits: " << hits << std::endl;
        std::cout << "Fails: " << fails << std::endl;


    }
}