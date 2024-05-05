#include "Fcm2.hpp"
#include "helpers.hpp"

namespace FCM {

    void Fcm2::printFCMCount() {
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
}