#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/argparser/argparser.hpp"

int main(int argc, char** argv){
    argc--;
    argv++;

    ArgParser::ArgParser parser(argc, argv);
    
    parser.registerArgType<int>("--k");
    parser.registerArgType<double>("--alpha");
    parser.registerArgType<double>("--threshold");
    parser.registerArgType<std::filesystem::path>("--inputFileName");

    parser.ParseArgs();

    int k = parser.GetArgValue<int>("--k");
    double alpha = parser.GetArgValue<double>("--alpha");
    double threshold = parser.GetArgValue<double>("--threshold");
    std::filesystem::path inputFileName = parser.GetArgValue<std::filesystem::path>("--inputFileName");

    std::ifstream fileSource;
    // Turn the exceptions bit on
    fileSource.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::string sequence;

    try{
        std::stringstream sequenceStream;

        fileSource.open(inputFileName);
        //TODO: Do not read the whole file into stack memory
        sequenceStream << fileSource.rdbuf();
        fileSource.close();

        sequence = sequenceStream.str();
    } catch(const std::ifstream::failure& e){
        std::cerr << "ERROR::SOURCE::FILE_NOT_SUCCESFULLY_READ" << '\n';
        return EXIT_FAILURE;
    }

    std::cout << "K: " << k << "\nThreshHold: " << threshold << "\nAlpha: " << alpha << "\nInput: " << inputFileName.string() << std::endl;
    return 0;
}