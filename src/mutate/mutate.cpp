#include "mutate.hpp"

namespace Mutate{
    char mutateChar(char c){
        char chars[] = {'A', 'C', 'G', 'T'};
        char newChar = chars[rand() % 4];
        while (newChar == c) {
            newChar = chars[rand() % 4];
        }
        return newChar;
    }

    void Mutator::ReadAlphabet(){
        decoder.readFile(inputFilePath);
    }

    void Mutator::PrintAlphabet(){
        decoder.printCodePoints();
    }
}
