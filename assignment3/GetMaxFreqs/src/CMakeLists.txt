cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Assignment3)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set compile options for all targets
add_compile_options(-Wall -W)

# Set build type specific compile options
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g3")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3")

# Set the output directory for binaries
set(BASE_DIR ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_DIR}/../bin)

add_executable(GetMaxFreqs GetMaxFreqs.cpp)
target_link_libraries(GetMaxFreqs sndfile FLAC vorbisfile vorbis ogg vorbisenc fftw3 m)

