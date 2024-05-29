cmake_minimum_required(VERSION 3.27)
project(TAI_P3)

set(CMAKE_CXX_STANDARD 17)

include_directories(bin/src/include)
include_directories(src/include)


add_executable(P3
        src/main.cpp
)
