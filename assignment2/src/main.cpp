#include <iostream>
#include <fstream>
#include "include/fcm.hpp"
#include "include/argparser/argparser.hpp"
#include "include/utils/Fcm2.hpp"
#include "include/utils/helpers.hpp"

int main(int argc, char **argv) {
    argc--;
    argv++;

    std::filesystem::path inputFileName;
    std::filesystem::path rhDataPath;
    std::filesystem::path rcDataPath;
    int kSize;
    double alpha;

    ArgParser::ArgParser parser(argc, argv);

    parser.registerArgType<int>("--kSize");
    parser.registerArgType<double>("--alpha");
    parser.registerArgType<std::filesystem::path>("--rhModelPath");
    parser.registerArgType<std::filesystem::path>("--rcModelPath");
    parser.registerArgType<std::filesystem::path>("--fileName");

    parser.ParseArgs();

    try{
        inputFileName = parser.GetArgValue<std::filesystem::path>("--fileName");
        rhDataPath = parser.GetArgValue<std::filesystem::path>("--rhModelPath");
        rcDataPath = parser.GetArgValue<std::filesystem::path>("--rcModelPath");
        kSize = parser.GetArgValue<int>("--kSize");
        alpha = parser.GetArgValue<double>("--alpha");
    } catch(const std::runtime_error& e){
        std::cerr << "Error missing arguments: " << e.what() << std::endl;
        parser.ShowHelp();
        return EXIT_FAILURE;
    }

    const auto& alphabet = UTILS::generateAsciiAlphabet();

    std::string testText = "This isn't a test. i dont know what to do";

    auto data = UTILS::readCSV(dataCSV);

    auto rhData = UTILS::filter(data, "0");
    auto rcData = UTILS::filter(data, "1");

    FCM::Fcm2 rhModel(kSize, alpha, alphabet);

    auto rhTexts = UTILS::getInput(rhData);
    for(auto& text: rhTexts) {
        rhModel.update(text);
    }
    rhModel.process();
    rhModel.evaluateTestText(testText);
    std::cout << "chegou aqui 1" << std::endl;

    FCM::Fcm2 rcModel(kSize, alpha, alphabet);

    auto rcTexts = UTILS::getInput(rcData);
    for(auto& text: rcTexts) {
        rhModel.update(text);
    }
    rcModel.process();
    rcModel.evaluateTestText(testText);
    std::cout << "chegou aqui 2" << std::endl;

    return EXIT_SUCCESS;
}