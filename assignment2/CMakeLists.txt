cmake_minimum_required(VERSION 3.27)
project(TAI)

set(CMAKE_CXX_STANDARD 17)

include_directories(bin/src/include)
include_directories(assignment2/src/include/utils)


add_executable(TAI
        assignment2/src/include/utils/CircularBuffer.hpp
        assignment2/src/include/fcm.hpp
        assignment2/src/fcm.cpp
        assignment2/src/main.cpp
        assignment2/src/include/utils/Fcm2.cpp
        assignment2/src/include/utils/Fcm2.hpp
        assignment2/src/include/utils/helpers.hpp
        assignment2/src/include/utils/helpers.cpp
        assignment2/src/include/argparser/argparser.cpp
        assignment2/src/include/argparser/argparser.hpp
)
