#include <iostream>
#include <string>
#include "argparser.hpp"
#include "../utils/utils.hpp"

namespace ArgParser{
    void ArgParser::ShowHelp() const {
        std::cout << "Options:\n";
            for (const auto& argType : argTypes) {
                std::cout << argType.first << ' ' << argType.second << "\n";
            }
    }

    void ArgParser::ParseArgs() {
        try {
            for (int i = 0; i < argc; ++i) {
                std::string argKey = argv[i];
                if (i + 1 < argc && conversionRegistry.find(argKey) != conversionRegistry.end()) {
                    std::string argValue = argv[i + 1];
                    argsMap[argKey] = {argKey, conversionRegistry[argKey](argValue)};
                }
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
            ShowHelp();
            exit(EXIT_FAILURE);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            ShowHelp();
            exit(EXIT_FAILURE);
        }
    };
}
