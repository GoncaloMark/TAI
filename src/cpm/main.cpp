#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/argparser/argparser.hpp"
#include "../include/utils/Utf8Reader.hpp"
#include "../include/utils/CircularBuffer.hpp"
#include "../include/utils/utf8Character.hpp"
#include "../include/cpm2.hpp"

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

    UTF8::Utf8Parser decoder(inputFileName, 1024);
    std::vector<uint32_t> buffer1;
    std::vector<uint32_t> buffer2;

    //TODO Maybe vector of references?
    std::vector<std::vector<uint32_t>> buffers = {buffer1, buffer2};
    

    decoder.readChunk(buffers, 1024);

    std::cout << "BUFFER1: " << buffers[0][20] << "\nBUFFER2: " << buffers[1][20] << std::endl;

    decoder.readChunk(buffers, 1024);

    std::cout << "BUFFER1: " << buffers[0][20] << "\nBUFFER2: " << buffers[1][20] << std::endl;

    decoder.readChunk(buffers, 1024);

    std::cout << "BUFFER1: " << buffers[0][20] << "\nBUFFER2: " << buffers[1][20] << std::endl;
    // CPM::CopyModel2 copyModel(inputFileName, alpha, threshold, k);
    // copyModel.process();


    std::cout << "K: " << k << "\nThreshHold: " << threshold << "\nAlpha: " << alpha << "\nInput: " << inputFileName.string() << std::endl;
    return 0;
}