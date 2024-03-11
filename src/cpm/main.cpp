#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/argparser/argparser.hpp"
#include "../include/utils/Utf8Reader.hpp"
#include "cpm.hpp"

int main(int argc, char** argv){
    argc--;
    argv++;

    ArgParser::ArgParser parser(argc, argv);
    
    parser.registerArgType<int>("--k");
    parser.registerArgType<double>("--alpha");
    parser.registerArgType<double>("--threshold");
    parser.registerArgType<std::filesystem::path>("--inputFileName");

    parser.ParseArgs();

    int k;
    double alpha;
    double threshold;
    std::filesystem::path inputFileName;

    try{
        k = parser.GetArgValue<int>("--k");
        alpha = parser.GetArgValue<double>("--alpha");
        threshold = parser.GetArgValue<double>("--threshold");
        inputFileName = parser.GetArgValue<std::filesystem::path>("--inputFileName");
    } catch(const std::runtime_error& e){
        std::cerr << "Error missing arguments: " << e.what() << std::endl;
        parser.ShowHelp();
        return EXIT_FAILURE;
    }

    //UTF8::Utf8Parser decoder(1024);

    UTF8::Utf8Reader reader(inputFileName);
    std::string symbol;
    reader.openFile();
    while(!reader.isEndOfFile()) {
        symbol = reader.getNextCharacter();
        if(reader.isEndOfFile()) {
            break;
        }
        std::cout << "->" << symbol << std::endl;
    }
    reader.closeFile();
    std::cout << "----------------------" << std::endl;


    std::cout << "K: " << k << "\nThreshHold: " << threshold << "\nAlpha: " << alpha << "\nInput: " << inputFileName.string() << std::endl;
    return 0;
}