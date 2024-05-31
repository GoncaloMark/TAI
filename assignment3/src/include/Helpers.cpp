//
// Created by renan on 31/05/24.
//

#include "Helpers.hpp"

namespace UTILS {
    int Helpers::parseArg(const char *arg) {
        char* end;
        errno = 0;
        long val = std::strtol(arg, &end, 10);
        if (errno != 0 || *end != '\0' || val < 0) {
            std::cerr << "Error: Invalid argument " << arg << std::endl;
            exit(EXIT_FAILURE);
        }
        return static_cast<int>(val);
    }
} // UTILS