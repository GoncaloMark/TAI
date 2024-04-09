#pragma once

#include <filesystem>
#include <unordered_map>
#include <variant>
#include <string>
#include <functional>

namespace ArgParser {
    /// @brief Variant of the values an Argument might take in the map.
    using ArgValue = std::variant<int, double, std::filesystem::path>;

    /// @brief Argument map type alias.
    using ArgTypeMap = std::unordered_map<std::string, std::string>;

    /// @struct
    /// @brief Arguments struct encapsulates the fields to be parsed by the argument parser
    typedef struct Argument {
        std::string name;
        ArgValue value;
    } args_t;

    /// @class
    /// @brief ArgParser class encapsulates the whole logics of parsing the arguments reading from the command line interface.
    /// @private argc - number of arguments
    /// @private argv - array of arguments
    class ArgParser {
        public:
            explicit ArgParser(int argc, char** argv) : argc(argc), argv(argv){};

            /// @brief Method to print out how to use this tool.
            void ShowHelp() const {
                std::cout << "Options:\n";
                    for (const auto& argType : argTypes) {
                        std::cout << argType.first << ' ' << argType.second << "\n";
                    }
            }

            /// @brief Method that iteratively parses the argumenst.
            /// @return args_t - returns by value a struct containing the parsed arguments.
            void ParseArgs();

            /// @brief This function registers arguments and its types.
            /// @tparam T Type argument for type registring notation.
            /// @param argName Name of the argument to be registered.
            template<typename T>
            void registerArgType(const std::string& argName) {
                conversionRegistry[argName] = [](const std::string& value) -> ArgValue {
                    if constexpr (std::is_same<T, int>::value) {
                        return std::stoi(value);
                    } else if constexpr (std::is_same<T, double>::value) {
                        return std::stod(value);
                    } else if constexpr (std::is_same<T, std::string>::value) {
                        return value;
                    } else if constexpr (std::is_same<T, std::filesystem::path>::value) {
                        return std::filesystem::path(value);
                    } else {
                        static_assert(std::is_same<T, int>::value || std::is_same<T, double>::value || std::is_same<T, std::string>::value || std::is_same<T, std::filesystem::path>::value, "Unsupported argument type.");
                    }
                };

                argTypes[argName] = getTypeName<T>();
            }

            /// @brief This function retrieves arguments from the map containing argument name and its value.
            /// @tparam T Type argument notates the type of the retrieved argument from the map.
            /// @param argKey This is the Key (argument name) stored in the map.
            /// @return Returns type argument of type T.
            template<typename T>
            T GetArgValue(const std::string& argKey) {
                if (argsMap.find(argKey) != argsMap.end()) {
                    return std::get<T>(argsMap[argKey].value);
                }
                throw std::runtime_error("Argument not found: " + argKey);
            }

        private:
            std::unordered_map<std::string, Argument> argsMap;
            std::unordered_map<std::string, std::function<ArgValue(const std::string&)>> conversionRegistry;
            ArgTypeMap argTypes;
            int argc;
            char** argv;

            /// @brief Private function to determine the argument type name for use in the help function.
            /// @tparam T Type paramater T for evaluating the type similarity.
            /// @return String representation of the evaluated type.
            template<typename T>
            std::string getTypeName() {
                if constexpr (std::is_same<T, int>::value) return "int";
                if constexpr (std::is_same<T, double>::value) return "double";
                if constexpr (std::is_same<T, std::filesystem::path>::value) return "path";
                if constexpr (std::is_same<T, std::string>::value) return "string";
                return "unknown";
            }
    };
}

