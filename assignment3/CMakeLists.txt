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
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src/include)

# List common source files
set(COMMON_SOURCES
        src/Helpers.cpp
        src/Compressor.cpp
        src/include/Helpers.hpp
        src/include/Constants.hpp
        src/include/Compressor.hpp
)

# Add executables
add_executable(SignaturesCreator src/SignaturesCreator.cpp ${COMMON_SOURCES})
add_executable(SegmentExtractor src/SegmentExtractor.cpp ${COMMON_SOURCES})
add_executable(MusicIdentifier src/MusicIdentifier.cpp ${COMMON_SOURCES})

# Link libraries for all executables
foreach(target IN ITEMS SignaturesCreator SegmentExtractor MusicIdentifier)
    target_link_libraries(${target} sox sndfile FLAC vorbisfile vorbis ogg vorbisenc fftw3 z lzma zstd bz2 m)
endforeach()
