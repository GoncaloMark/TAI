#include <iostream>
#include <fstream>
#include "include/fcm.hpp"
#include "include/argparser/argparser.hpp"

int main(int argc, char **argv){
    argc--;
    argv++;

    std::string asciiChars;
    for (int i = 0; i < 128; ++i) {
        asciiChars += static_cast<char>(i);
    }

    std::filesystem::path inputFileName;
    int kSize;
    double alpha;

    ArgParser::ArgParser parser(argc, argv);

    parser.registerArgType<int>("--kSize");
    parser.registerArgType<double>("--alpha");
    parser.registerArgType<std::filesystem::path>("--fileName");

    parser.ParseArgs();

    try{
        inputFileName = parser.GetArgValue<std::filesystem::path>("--fileName");
        kSize = parser.GetArgValue<int>("--kSize");
        alpha = parser.GetArgValue<double>("--alpha");
    } catch(const std::runtime_error& e){
        std::cerr << "Error missing arguments: " << e.what() << std::endl;
        parser.ShowHelp();
        return EXIT_FAILURE;
    }

    std::string testText = "This isn't a test.";

    FCM::Model humanModel(alpha, kSize, inputFileName, asciiChars);
    humanModel.process();

    humanModel.evaluateTestText(testText);

    // FCM::Model gptModel(alpha, kSize, inputFileName, asciiChars);
    // gptModel.process();


    return EXIT_SUCCESS;
}