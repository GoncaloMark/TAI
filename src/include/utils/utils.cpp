#include <string>
#include "utils.hpp"

inline double calculateHitProbability(const unsigned int Nh, const unsigned int Nf, const double alpha){
    return static_cast<double>((Nh + alpha) / (Nh + Nf + 2*alpha));
}