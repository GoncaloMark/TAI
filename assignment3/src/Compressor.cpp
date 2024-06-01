#include "include/Compressor.hpp"


namespace COMPRESSOR {

    std::vector<unsigned char> compressGzip(const std::vector<unsigned char>& data) {
        uLongf compressedSize = compressBound(data.size());
        std::vector<unsigned char> compressedData(compressedSize);

        if (compress2(compressedData.data(), &compressedSize, data.data(), data.size(), Z_BEST_COMPRESSION) != Z_OK) {
            throw std::runtime_error("GZIP compression failed");
        }
        compressedData.resize(compressedSize);
        return compressedData;
    }

    std::vector<unsigned char> compressBzip2(const std::vector<unsigned char>& data) {
        unsigned int compressedSize = data.size() * 1.01 + 600;
        std::vector<unsigned char> compressedData(compressedSize);

        if (BZ2_bzBuffToBuffCompress(reinterpret_cast<char*>(compressedData.data()), &compressedSize, const_cast<char*>(reinterpret_cast<const char*>(data.data())), data.size(), 9, 0, 30) != BZ_OK) {
            throw std::runtime_error("BZIP2 compression failed");
        }
        compressedData.resize(compressedSize);
        return compressedData;
    }

    std::vector<unsigned char> compressLzma(const std::vector<unsigned char>& data) {
        size_t compressedSize = data.size() + data.size() / 3 + 128;
        std::vector<unsigned char> compressedData(compressedSize);

        lzma_stream strm = LZMA_STREAM_INIT;
        if (lzma_easy_encoder(&strm, LZMA_PRESET_DEFAULT, LZMA_CHECK_CRC64) != LZMA_OK) {
            throw std::runtime_error("LZMA compression initialization failed");
        }

        strm.next_in = data.data();
        strm.avail_in = data.size();
        strm.next_out = compressedData.data();
        strm.avail_out = compressedSize;

        if (lzma_code(&strm, LZMA_FINISH) != LZMA_STREAM_END) {
            lzma_end(&strm);
            throw std::runtime_error("LZMA compression failed");
        }

        compressedData.resize(compressedSize - strm.avail_out);
        lzma_end(&strm);
        return compressedData;
    }

    std::vector<unsigned char> compressZstd(const std::vector<unsigned char>& data) {
        size_t compressedSize = ZSTD_compressBound(data.size());
        std::vector<unsigned char> compressedData(compressedSize);

        size_t result = ZSTD_compress(compressedData.data(), compressedSize, data.data(), data.size(), 1);
        if (ZSTD_isError(result)) {
            throw std::runtime_error("ZSTD compression failed");
        }
        compressedData.resize(result);
        return compressedData;
    }

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

    std::vector<unsigned char> compress(const std::vector<unsigned char>& data1, const std::vector<unsigned char>& data2, CompressionMethod method) {
        std::vector<unsigned char> combinedData = data1;
        combinedData.insert(combinedData.end(), data2.begin(), data2.end());
        return compress(combinedData, method);
    }

}
