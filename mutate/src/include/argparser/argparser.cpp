#include "argparser.hpp"
#include <iostream>
#include <string>

void ArgParser::ShowHelp() const {
    std::cout << "cmp: cmp [-akf]\n" \
    << "Options:\n" \
    << "-c \t Chance of changing file\n" \
    << "-i \t File with target sequence\n" \
    << "-h \t Print this help message" << std::endl;
}

bool ArgParser::isNumber(char* s) const{
    int result = atoi(s);
    if (result == 0 ) {
        return false;
    }
    return true;
}

args_t ArgParser::ParseArgs() const {
    if (argc == 0) {
        std::cerr << "No arguments were entered." << std::endl;
        ArgParser::ShowHelp();
        exit(EXIT_FAILURE);
    }

    args_t args;

    args.mutationChance = 0;

    for (int i = 0; i < argc; ++i) {
        std::string local_argv = argv[i];
        if (local_argv.compare("-h") == 0 || local_argv.compare("--help") == 0) {
            ArgParser::ShowHelp();
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < argc; ++i) {
        std::string local_argv = argv[i];

        if (local_argv.compare("-c") == 0 || local_argv.compare("--chance") == 0) {

            if (isNumber(argv[i + 1])){
                args.mutationChance = atof(argv[i + 1]);
            }

        } else if (local_argv.compare("-i") == 0 || local_argv.compare("--input") == 0) {

            args.inputFilePath = argv[i + 1];

        }
    }

    if (args.mutationChance == 0) {
        std::cout << "Alpha was not provided. Using default value: 1" << std::endl;
        args.mutationChance = 50;
    }

    return args;
};