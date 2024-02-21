#include <iostream>

int main(int argc, char** argv){
    argc--;
    argv++;

    std::cout << "N_Args: " << argc << std::endl;
    return 0;
}