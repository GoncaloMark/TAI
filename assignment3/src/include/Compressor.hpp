#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <zlib.h>
#include <bzlib.h>
#include <lzma.h>
#include <zstd.h>

namespace COMPRESSOR {
    enum class CompressionMethod {
        GZIP,
        BZIP2,
        LZMA,
        ZSTD
    };

    std::vector<unsigned char> compress(const std::vector<unsigned char>& data, CompressionMethod method);
    std::vector<unsigned char> compress(const std::vector<unsigned char>& data1, const std::vector<unsigned char>& data2, CompressionMethod method);
}

