#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/argparser/argparser.hpp"
#include "../include/utils/Utf8Reader.hpp"
#include "../include/utils/CircularBuffer.hpp"
#include "../include/utils/utf8Character.hpp"
#include "../include/cpm.hpp"
#include "../include/utils/SourceInfo.hpp"

int main(int argc, char** argv){
    argc--;
    argv++;

    ArgParser::ArgParser parser(argc, argv);
    
    parser.registerArgType<int>("--k");
    parser.registerArgType<int>("--bufSize");
    parser.registerArgType<double>("--alpha");
    parser.registerArgType<double>("--threshold");
    parser.registerArgType<std::filesystem::path>("--inputFileName");

    parser.ParseArgs();

    int k, bufSize;
    double alpha, threshold;
    std::filesystem::path inputFileName;

    try{
        k = parser.GetArgValue<int>("--k");
        bufSize = parser.GetArgValue<int>("--bufSize");
        alpha = parser.GetArgValue<double>("--alpha");
        threshold = parser.GetArgValue<double>("--threshold");
        inputFileName = parser.GetArgValue<std::filesystem::path>("--inputFileName");
    } catch(const std::runtime_error& e){
        std::cerr << "Error missing arguments: " << e.what() << std::endl;
        parser.ShowHelp();
        return EXIT_FAILURE;
    }

    UTF8::Utf8Parser decoder(inputFileName, bufSize);
    FILEINFO::SourceInfo sourceInfo = FILEINFO::SourceInfo::getInformation(decoder);
    CPM::CopyModel copyModel(alpha, threshold, k, bufSize, decoder, sourceInfo);
    copyModel.start();

    std::cout << "size: " << sourceInfo.size() << std::endl;
    std::cout << "entropy: " << sourceInfo.getEntropy() << std::endl;
    std::cout << "TotalBits: " << copyModel.getTotalBits() << std::endl;

    // std::cout << "K: " << k << "\nThreshHold: " << threshold << "\nAlpha: " << alpha << "\nInput: " << inputFileName.string() << std::endl;
    return 0;
}