#include <iostream>
#include <fstream>
#include "include/fcm.hpp"
#include "include/argparser/argparser.hpp"
#include "include/utils/Fcm2.hpp"
#include "include/utils/helpers.hpp"

int main(int argc, char **argv) {
    argc--;
    argv++;

    std::filesystem::path testTextPath;
    std::filesystem::path rhDataPath;
    std::filesystem::path rcDataPath;
    int kSize;
    double alpha;

    ArgParser::ArgParser parser(argc, argv);

    parser.registerArgType<int>("--kSize");
    parser.registerArgType<double>("--alpha");
    parser.registerArgType<std::filesystem::path>("--rhModelPath");
    parser.registerArgType<std::filesystem::path>("--rcModelPath");
    parser.registerArgType<std::filesystem::path>("--textFile");

    parser.ParseArgs();
    try{
        testTextPath = parser.GetArgValue<std::filesystem::path>("--textFile");
        rhDataPath = parser.GetArgValue<std::filesystem::path>("--rhModelPath");
        rcDataPath = parser.GetArgValue<std::filesystem::path>("--rcModelPath");
        kSize = parser.GetArgValue<int>("--kSize");
        alpha = parser.GetArgValue<double>("--alpha");
    } catch(const std::runtime_error& e){
        std::cerr << "Error missing arguments: " << e.what() << std::endl;
        parser.ShowHelp();
        return EXIT_FAILURE;
    }

    // Program Execution
    const auto& alphabet = UTILS::generateAsciiAlphabet();
    std::string testText = UTILS::readText(testTextPath);

    FCM::FCMModel rcModel = FCM::FCMModel::buildModel(kSize, alpha, alphabet, rcDataPath);
    FCM::FCMModel rhModel = FCM::FCMModel::buildModel(kSize, alpha, alphabet, rhDataPath);

    std::cout << std::endl;
    std::cout << "Rh Model: " << std::endl;
    rhModel.evaluateText(testText);
    std::cout << std::endl;
    std::cout << "Rc Model: " << std::endl;
    rcModel.evaluateText(testText);
    std::cout << std::endl;

    if(FCM::FCMModel::wasRewrittenChatGpt(testText, rhModel, rcModel)) {
        std::cout << "The text is likely rewritten by ChatGpt" << std::endl;
    } else {
        std::cout << "The text is likely rewritten by a Human User" << std::endl;
    }


    return EXIT_SUCCESS;
}