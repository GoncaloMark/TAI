#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <zlib.h>
#include <bzlib.h>
#include <lzma.h>
#include <zstd.h>

namespace COMPRESSOR {

    /**
     * Enum representing the available compression methods.
     */
    enum class CompressionMethod {
        GZIP,   // gzip compression
        BZIP2,  // bzip2 compression
        LZMA,   // lzma compression
        ZSTD    // zstd compression
    };

    /**
     * Function to compress a single data vector using the specified compression method.
     *
     * This function takes a vector of data and compresses it using the specified compression method.
     * It returns the compressed data as a vector of unsigned char.
     *
     * @param data The input data vector to be compressed.
     * @param method The compression method to use (GZIP, BZIP2, LZMA, or ZSTD).
     * @return A vector of unsigned char containing the compressed data.
     * @throws std::runtime_error if the compression fails.
     */
    std::vector<unsigned char> compress(const std::vector<unsigned char>& data, CompressionMethod method);

    /**
     * Function to compress two concatenated data vectors using the specified compression method.
     *
     * This function takes two vectors of data, concatenates them, and compresses the combined data
     * using the specified compression method. It returns the compressed data as a vector of unsigned char.
     *
     * @param data1 The first input data vector to be compressed.
     * @param data2 The second input data vector to be compressed.
     * @param method The compression method to use (GZIP, BZIP2, LZMA, or ZSTD).
     * @return A vector of unsigned char containing the compressed data.
     * @throws std::runtime_error if the compression fails.
     */
    std::vector<unsigned char> compress(const std::vector<unsigned char>& data1, const std::vector<unsigned char>& data2, CompressionMethod method);

} // namespace COMPRESSOR
