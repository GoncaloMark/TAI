#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/argparser/argparser.hpp"
#include "mutate.hpp"

int main(int argc, char** argv){
    argc--;
    argv++;

    double prob;
    std::filesystem::path inputFileName;
    std::filesystem::path outputFileName;

    ArgParser::ArgParser parser(argc, argv);

    parser.registerArgType<double>("--probability");
    parser.registerArgType<std::filesystem::path>("--inputFileName");
    parser.registerArgType<std::filesystem::path>("--outputFileName");

    parser.ParseArgs();

    try{
        prob = parser.GetArgValue<double>("--probability") / 100.0;
        inputFileName = parser.GetArgValue<std::filesystem::path>("--inputFileName");
        outputFileName = parser.GetArgValue<std::filesystem::path>("--outputFileName");
    } catch(const std::runtime_error& e){
        std::cerr << "Error missing arguments: " << e.what() << std::endl;
        parser.ShowHelp();
        return EXIT_FAILURE;
    }

    UTF8::Utf8Parser decoder(1024);

    Mutate::Mutator mutate(inputFileName, outputFileName, prob, decoder);
    mutate.ReadAlphabet();
    mutate.MutateFile();

    return EXIT_SUCCESS;
}