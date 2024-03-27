#include "Helpers.hpp"

namespace UTILS {
    double Helpers::calculateEntropy(double frequency) {
        return -log2(frequency);
    }
} // UTILS