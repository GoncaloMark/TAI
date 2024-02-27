#include <iostream>
#include <string>
#include "argparser.hpp"
#include "../utils/utils.hpp"

void ArgParser::ShowHelp() const {
    std::cout << "cmp: cmp [-aktih]\n" \
    << "Options:\n" \
    << "-a \t Alpha\n" \
    << "-k \t Window size\n" \
    << "-t \t Threshold\n" \
    << "-i \t File with target sequence\n" \
    << "-h \t Print this help message" << std::endl;
}

cpm_args_t ArgParser::ParseArgs() const {
    if (argc == 0) {
        std::cerr << "No arguments were entered." << std::endl;
        ArgParser::ShowHelp();
        exit(EXIT_FAILURE);
    }

    cpm_args_t args;

    args.k = 0;
    args.alpha = 0;
    args.threshHold = 0;

    for (int i = 0; i < argc; ++i) {
        std::string local_argv = argv[i];
        if (local_argv.compare("-h") == 0 || local_argv.compare("--help") == 0) {
            ArgParser::ShowHelp();
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < argc; ++i) {
        std::string local_argv = argv[i];

        if (local_argv.compare("-a") == 0 || local_argv.compare("--alpha") == 0) {

            if (isNumber(argv[i + 1])){
                args.alpha = atof(argv[i + 1]);
            }

        } else if (local_argv.compare("-k") == 0) {

            if (isNumber(argv[i + 1])){
                args.k = atoi(argv[i + 1]);
            }

        } else if (local_argv.compare("-i") == 0 || local_argv.compare("--input") == 0) {

            args.inputFilePath = argv[i + 1];

        } else if (local_argv.compare("-t") == 0 || local_argv.compare("--threshold") == 0) {

            if (isNumber(argv[i + 1])){
                args.threshHold = atof(argv[i + 1]);
            }
        }
    }

    if (args.alpha == 0) {
        std::cout << "Alpha was not provided. Using default value: 1" << std::endl;
        args.alpha = 1;
    }

    if (args.k == 0) {
        std::cout << "Window size was not provided. Using default value: 3" << std::endl;
        args.k = 3;
    }

    if (args.threshHold == 0) {
        std::cout << "Threshold was not provided. Using default value: 0.4" << std::endl;
        args.threshHold = 0.4;
    }

    return args;
};