cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Assignment3)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W")
SET(CMAKE_C_FLAGS_RELEASE "-O3")
SET(CMAKE_C_FLAGS_DEBUG "-g3")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -std=c++11")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_FLAGS_DEBUG "-g3")

SET(BASE_DIR ${CMAKE_SOURCE_DIR} )
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_DIR}/../bin)

add_executable(GetMaxFreqs GetMaxFreqs.cpp)
target_link_libraries(GetMaxFreqs -lsndfile -lFLAC -lvorbisfile -lvorbis -logg -lvorbisenc -lfftw3 -lm -static)

