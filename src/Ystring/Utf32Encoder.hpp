//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-11-21
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Ystring/EncoderBase.hpp"
#include "Ystring/UnicodeChars.hpp"

namespace Ystring
{
    template <bool SwapBytes>
    class Utf32Encoder : public EncoderBase
    {
    public:
        Utf32Encoder()
            : EncoderBase(IsBigEndian == SwapBytes
                          ? Encoding::UTF_32_LE
                          : Encoding::UTF_32_BE)
        {}

        size_t getEncodedSize(const char32_t* src, size_t srcSize) override
        {
            size_t result = 0;
            for (size_t i = 0; i < srcSize; ++i)
            {
                if (src[i] <= UNICODE_MAX)
                    result += 4;
            }
            return result;
        }

        std::pair<size_t, size_t>
        encode(const char32_t* src, size_t srcSize,
               void* dst, size_t dstSize) override
        {
            auto cdst = static_cast<char*>(dst);
            size_t bytesWritten = 0;
            for (size_t i = 0; i < srcSize; ++i)
            {
                if (src[i] <= UNICODE_MAX)
                {
                    if (dstSize < 4)
                        return {i, bytesWritten};
                }
                union
                {
                    char32_t c;
                    char b[4];
                } u = {swapEndianness<SwapBytes>(src[i])};
                *out++ = u.b[0];
                *out++ = u.b[1];
                *out++ = u.b[2];
                *out++ = u.b[3];
                bytesWritten += n;
            }
            return {srcSize, bytesWritten};
        }

        size_t encode(const char32_t* src, size_t srcSize,
                      std::string& dst) override
        {
            auto out = back_inserter(dst);
            for (size_t i = 0; i < srcSize; ++i)
            {
                if (src[i] <= UNICODE_MAX)
                {
                    union {char32_t c; char b[4];} u = {swapEndianness<SwapBytes>(src[i])};
                    *out++ = u.b[0];
                    *out++ = u.b[1];
                    *out++ = u.b[2];
                    *out++ = u.b[3];
                }
            }
            return srcSize * 4;
        }
    };

    typedef Utf32Encoder<IsLittleEndian> Utf32BEEncoder;
    typedef Utf32Encoder<IsBigEndian> Utf32LEEncoder;
}
