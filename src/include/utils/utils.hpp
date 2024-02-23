#pragma once

/// @brief          Function that calculates the probabilities of the hit.
/// @param Nh       Number of Hits
/// @param Nf       Number of Fails
/// @param alpha    Alpha term
/// @return         The calculted probability
inline double calculateHitProbability(const unsigned int Nh, const unsigned int Nf, const double alpha);

/// @brief      Verifies if input argument is a number
/// @param s    pointer to characters to be read and verify if its a number.
/// @return     true if s is a number, false if it's not a number.
bool isNumber(char* s);