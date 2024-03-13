#pragma once

#include <vector>
#include <string>

namespace UTILS {

    /// @brief          Function that calculates the probabilities of the hit.
    /// @param nHits       Number of Hits
    /// @param nFails       Number of Fails
    /// @param alpha    Alpha term
    /// @return         The calculated probability
    double calculateHitProbability(const unsigned int nHits, const unsigned int nFails, const double alpha) {
        return static_cast<double>((nHits + alpha) / (nHits + nFails + 2 * alpha));
    };

    std::string toString(const std::vector<std::string>& arr) {
        std::string str;
        for(const auto & character : arr) {
            str += character;
        }
        return str;
    };
}
