//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-11-06
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Utf8Encoder.hpp"

#include <iterator>
#include "Ystring/EncodeUtf8.hpp"

namespace Ystring
{
    Utf8Encoder::Utf8Encoder()
        : EncoderBase(Encoding::UTF8)
    {}

    std::pair<size_t, size_t>
    Utf8Encoder::encode(const char32_t* src, size_t srcSize,
                        void* dst, size_t dstSize)
    {
        auto cdst = static_cast<char*>(dst);
        size_t bytesWritten = 0;
        for (size_t i = 0; i < srcSize; ++i)
        {
            auto n = encodeUtf8(src[i], cdst + bytesWritten, dstSize - bytesWritten);
            if (n == 0)
                return {i, bytesWritten};
            bytesWritten += n;
        }
        return {srcSize, bytesWritten};
    }

    size_t Utf8Encoder::encode(const char32_t* src, size_t srcSize,
                               std::string& dst)
    {
        auto out = back_inserter(dst);
        for (size_t i = 0; i < srcSize; ++i)
            encodeUtf8(src[i], out);
        return srcSize;
    }
}
