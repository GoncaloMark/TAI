#pragma once
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <filesystem>

namespace FCM {
    class Model {
        public:
            explicit Model(const double alpha, const int k, const std::filesystem::path& filePath, const std::string& alphabetStr)
                : alpha(alpha), kSize(k), fileSource(filePath) {
                    int index = 0;
                    for (auto c : alphabetStr) {
                        alphabet.insert(c);
                        charToIndex[c] = index++;
                    }
                };

            void process();
            void evaluateTestText(const std::string& testText);

        private:
            const double alpha;
            const int kSize;
            std::unordered_set<char> alphabet;
            std::unordered_map<char, int> charToIndex;
            std::unordered_map<std::string, std::vector<int>> counting;
            std::unordered_map<std::string, std::vector<double>> probabilities;
            const std::filesystem::path fileSource;

            void calculateProbabilities();
            double calculateEntropyForText(const std::string& text);

            void printCounting(){
                for (const auto& pair : counting) {
                    std::cout << "CTX: " << pair.first; //<< " | Counts: ";
                    // for (int count : pair.second) {
                    //     std::cout << count << " ";
                    // }
                    std::cout << std::endl;
                }
            }
            

    };
}