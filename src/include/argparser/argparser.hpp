#pragma once

/// @brief      Arguments struct encapsulates the fields to be parsed by the argument parser
/// @property   k - Sliding Window size
/// @property   threshHold - Maximum margin for fails
/// @property   alpha - Control term
/// @property   inputFilePath - file path to read
typedef struct Arguments {
    int k;
    double threshHold;
    double alpha;
    std::filesystem::path inputFilePath;
} args_t;

/// @brief      ArgParser class encapsulates the whole logics of parsing the arguments reading from the command line interface.
/// @private    argc - number of arguments
/// @private    argv - array of arguments
class ArgParser {
    public:
        ArgParser(int argc, char** argv) : argc(argc), argv(argv){};

        /// @brief Method to print out how to use this tool.
        void ShowHelp() const;

        /// @brief      Method that iteratively parses the argumenst.
        /// @return     args_t - returns by value a struct containing the parsed arguments.
        args_t ParseArgs() const;

    private:
        int argc;
        char** argv;
};
