#include "utils.hpp"

inline double calculateHitProbability(const unsigned int Nh, const unsigned int Nf, const double alpha){
    return (double) (Nh + alpha) / (Nh + Nf + 2*alpha);
}