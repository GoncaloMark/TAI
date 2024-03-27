#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "../include/argparser/argparser.hpp"
#include "mutate.hpp"

int main(int argc, char** argv){
    argc--;
    argv++;

    double prob;
    int bufferSize;
    std::filesystem::path inputFileName;
    std::filesystem::path outputFileName;

    ArgParser::ArgParser parser(argc, argv);

    parser.registerArgType<double>("--probability");
    parser.registerArgType<int>("--bufferSize");
    parser.registerArgType<std::filesystem::path>("--inputFileName");
    parser.registerArgType<std::filesystem::path>("--outputFileName");

    parser.ParseArgs();

    try{
        prob = parser.GetArgValue<double>("--probability") / 100.0;
        bufferSize = parser.GetArgValue<int>("--bufferSize");
        inputFileName = parser.GetArgValue<std::filesystem::path>("--inputFileName");
        outputFileName = parser.GetArgValue<std::filesystem::path>("--outputFileName");
    } catch(const std::runtime_error& e){
        std::cerr << "Error missing arguments: " << e.what() << std::endl;
        parser.ShowHelp();
        return EXIT_FAILURE;
    }

    auto beg = std::chrono::high_resolution_clock::now();
    UTF8::Utf8Parser decoder(inputFileName, bufferSize);

    Mutate::Mutator mutate(outputFileName, prob, decoder);
    mutate.mutateFile(bufferSize);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - beg);

    std::cout << "Elapsed Time: " << duration.count() << "s" << std::endl;

    return EXIT_SUCCESS;
}