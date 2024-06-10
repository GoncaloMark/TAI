#include "include/Compressor.hpp"

namespace COMPRESSOR {

    /**
     * Function to compress data using gzip.
     *
     * @param data The input data vector to be compressed.
     * @return A vector of unsigned char containing the compressed data.
     * @throws std::runtime_error if the compression fails.
     */
    std::vector<unsigned char> compressGzip(const std::vector<unsigned char>& data) {
        // Calculate the maximum size of the compressed data
        uLongf compressedSize = compressBound(data.size());
        std::vector<unsigned char> compressedData(compressedSize);

        // Perform the compression
        if (compress2(compressedData.data(), &compressedSize, data.data(), data.size(), Z_BEST_COMPRESSION) != Z_OK) {
            throw std::runtime_error("GZIP compression failed");
        }

        // Resize the vector to the actual compressed size
        compressedData.resize(compressedSize);
        return compressedData;
    }

    /**
     * Function to compress data using bzip2.
     *
     * @param data The input data vector to be compressed.
     * @return A vector of unsigned char containing the compressed data.
     * @throws std::runtime_error if the compression fails.
     */
    std::vector<unsigned char> compressBzip2(const std::vector<unsigned char>& data) {
        // Calculate the maximum size of the compressed data
        auto compressedSize = static_cast<unsigned int>(static_cast<double>(data.size()) * 1.01 + 600);
        std::vector<unsigned char> compressedData(compressedSize);

        // Perform the compression
        if (BZ2_bzBuffToBuffCompress(reinterpret_cast<char*>(compressedData.data()), &compressedSize, 
                                     const_cast<char*>(reinterpret_cast<const char*>(data.data())), data.size(), 9, 0, 30) != BZ_OK) {
            throw std::runtime_error("BZIP2 compression failed");
        }

        // Resize the vector to the actual compressed size
        compressedData.resize(compressedSize);
        return compressedData;
    }

    /**
     * Function to compress data using lzma.
     *
     * @param data The input data vector to be compressed.
     * @return A vector of unsigned char containing the compressed data.
     * @throws std::runtime_error if the compression fails.
     */
    std::vector<unsigned char> compressLzma(const std::vector<unsigned char>& data) {
        // Calculate the maximum size of the compressed data
        size_t compressedSize = data.size() + data.size() / 3 + 128;
        std::vector<unsigned char> compressedData(compressedSize);

        // Initialize the lzma stream
        lzma_stream strm = LZMA_STREAM_INIT;
        if (lzma_easy_encoder(&strm, LZMA_PRESET_DEFAULT, LZMA_CHECK_CRC64) != LZMA_OK) {
            throw std::runtime_error("LZMA compression initialization failed");
        }

        // Set input and output buffers
        strm.next_in = data.data();
        strm.avail_in = data.size();
        strm.next_out = compressedData.data();
        strm.avail_out = compressedSize;

        // Perform the compression
        if (lzma_code(&strm, LZMA_FINISH) != LZMA_STREAM_END) {
            lzma_end(&strm);
            throw std::runtime_error("LZMA compression failed");
        }

        // Resize the vector to the actual compressed size
        compressedData.resize(compressedSize - strm.avail_out);
        lzma_end(&strm);
        return compressedData;
    }

    /**
     * Function to compress data using zstd.
     *
     * @param data The input data vector to be compressed.
     * @return A vector of unsigned char containing the compressed data.
     * @throws std::runtime_error if the compression fails.
     */
    std::vector<unsigned char> compressZstd(const std::vector<unsigned char>& data) {
        // Calculate the maximum size of the compressed data
        size_t compressedSize = ZSTD_compressBound(data.size());
        std::vector<unsigned char> compressedData(compressedSize);

        // Perform the compression
        size_t result = ZSTD_compress(compressedData.data(), compressedSize, data.data(), data.size(), 1);
        if (ZSTD_isError(result)) {
            throw std::runtime_error("ZSTD compression failed");
        }

        // Resize the vector to the actual compressed size
        compressedData.resize(result);
        return compressedData;
    }

    /**
     * Function to compress a single data vector using the specified compression method.
     *
     * @param data The input data vector to be compressed.
     * @param method The compression method to use (GZIP, BZIP2, LZMA, or ZSTD).
     * @return A vector of unsigned char containing the compressed data.
     * @throws std::runtime_error if the compression fails.
     */
    std::vector<unsigned char> compress(const std::vector<unsigned char>& data, CompressionMethod method) {
        switch (method) {
            case CompressionMethod::GZIP:
                return compressGzip(data);
            case CompressionMethod::BZIP2:
                return compressBzip2(data);
            case CompressionMethod::LZMA:
                return compressLzma(data);
            case CompressionMethod::ZSTD:
                return compressZstd(data);
            default:
                throw std::invalid_argument("Unsupported compression method");
        }
    }

    /**
     * Function to compress two concatenated data vectors using the specified compression method.
     *
     * @param data1 The first input data vector to be compressed.
     * @param data2 The second input data vector to be compressed.
     * @param method The compression method to use (GZIP, BZIP2, LZMA, or ZSTD).
     * @return A vector of unsigned char containing the compressed data.
     * @throws std::runtime_error if the compression fails.
     */
    std::vector<unsigned char> compress(const std::vector<unsigned char>& data1, const std::vector<unsigned char>& data2, CompressionMethod method) {
        // Combine the two data vectors
        std::vector<unsigned char> combinedData = data1;
        combinedData.insert(combinedData.end(), data2.begin(), data2.end());

        // Compress the combined data
        return compress(combinedData, method);
    }

}
