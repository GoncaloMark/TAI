#include "Helpers.hpp"

namespace UTILS {
    double Helpers::calculateEntropy(double frequency) {
        return -frequency*log2(frequency);
    }
} // UTILS