#pragma once

/// @brief          Function that calculates the probabilities of the hit.
/// @param Nh       Number of Hits
/// @param Nf       Number of Fails
/// @param alpha    Alpha term
/// @return         The calculted probability
inline double calculateHitProbability(const unsigned int Nh, const unsigned int Nf, const double alpha);