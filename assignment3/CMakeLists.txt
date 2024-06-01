cmake_minimum_required(VERSION 3.10)
project(TAI_P3)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
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
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/src/include)

add_executable(GetMaxFreqs src/GetMaxFreqs.cpp
        src/include/Helpers.hpp
        src/include/Constants.hpp
        src/Helpers.cpp)

add_executable(MusicSigExtractor src/MusicSigExtractor.cpp
        src/include/Helpers.hpp
        src/include/Constants.hpp
        src/Helpers.cpp)

add_executable(SegmentCreator src/SegmentCreator.cpp
        src/include/Helpers.hpp
        src/include/Constants.hpp
        src/Helpers.cpp)


target_link_libraries(GetMaxFreqs sndfile FLAC vorbisfile vorbis ogg vorbisenc fftw3 m)
target_link_libraries(MusicSigExtractor sndfile FLAC vorbisfile vorbis ogg vorbisenc fftw3 m)
target_link_libraries(SegmentCreator sndfile FLAC vorbisfile vorbis ogg vorbisenc fftw3 m)
