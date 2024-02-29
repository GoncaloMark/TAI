#include "mutate.hpp"

char mutateChar(char c){
    char chars[] = {'A', 'C', 'G', 'T'};
    char newChar = chars[rand() % 4];
    while (newChar == c) {
        newChar = chars[rand() % 4];
    }
    return newChar;
}