#pragma once
#include <filesystem>

/// @struct
/// @brief Arguments struct encapsulates the fields to be parsed by the argument parser
typedef struct CPMArguments {
    int k; /**< Sliding Window size */
    double threshHold; /**< Control term */
    double alpha; /**< Maximum margin for fails */
    std::filesystem::path inputFilePath; /**< file path to read */
} cpm_args_t;

/// @class
/// @brief ArgParser class encapsulates the whole logics of parsing the arguments reading from the command line interface.
/// @private argc - number of arguments
/// @private argv - array of arguments
class ArgParser {
    public:
        ArgParser(int argc, char** argv) : argc(argc), argv(argv){};

        /// @brief Method to print out how to use this tool.
        void ShowHelp() const;

        /// @brief Method that iteratively parses the argumenst.
        /// @return args_t - returns by value a struct containing the parsed arguments.
        cpm_args_t ParseArgs() const;

    private:
        int argc;
        char** argv;
};
