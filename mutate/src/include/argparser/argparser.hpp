#pragma once
#include <filesystem>

/// @brief      Arguments struct encapsulates the fields to be parsed by the argument parser
/// @property   mutationChance - Chance of changing the file
/// @property   inputFilePath - file path to read
typedef struct Arguments {
    double mutationChance;
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
        /// @brief      Verifies if input argument is a number
        /// @param s    pointer to characters to be read and verify if its a number.
        /// @return     true if s is a number, false if it's not a number.
        bool isNumber(char* s) const;
        
        int argc;
        char** argv;
};
