#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/argparser/argparser.hpp"
#include "mutate.hpp"

int main(int argc, char** argv){
    argc--;
    argv++;

    ArgParser::ArgParser parser(argc, argv);

    parser.registerArgType<double>("--probability");
    parser.registerArgType<std::filesystem::path>("--inputFileName");
    parser.registerArgType<std::filesystem::path>("--outputFileName");

    parser.ParseArgs();

    double prob = parser.GetArgValue<double>("--probability") / 100.0;
    std::filesystem::path inputFileName = parser.GetArgValue<std::filesystem::path>("--inputFileName");
    std::filesystem::path outputFileName = parser.GetArgValue<std::filesystem::path>("--outputFileName");

    return EXIT_SUCCESS;
}