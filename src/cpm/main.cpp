#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/argparser/argparser.hpp"

int main(int argc, char** argv){
    argc--;
    argv++;

    ArgParser parser(argc, argv);
    const cpm_args_t args = parser.ParseArgs();

    std::ifstream fileSource;
    // Turn the exceptions bit on
    fileSource.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::string sequence;

    try{
        std::stringstream sequenceStream;

        fileSource.open(args.inputFilePath);
        //TODO: Do not read the whole file into stack memory
        sequenceStream << fileSource.rdbuf();
        fileSource.close();

        sequence = sequenceStream.str();
    } catch(const std::ifstream::failure& e){
        std::cerr << "ERROR::SOURCE::FILE_NOT_SUCCESFULLY_READ" << '\n';
        return EXIT_FAILURE;
    }

    std::cout << "K: " << args.k << "\nThreshHold: " << args.threshHold << "\nAlpha: " << args.alpha << "\nInput: " << args.inputFilePath.string() << std::endl;
    return 0;
}