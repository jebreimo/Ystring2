//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-06-29.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Ystring/EncoderBase.hpp"
#include "Ystring/EncodeUtf16.hpp"

namespace Ystring
{
    template <bool SwapBytes>
    class Utf16Encoder : public EncoderBase
    {
    public:
        Utf16Encoder()
            : EncoderBase(IsBigEndian == SwapBytes
                          ? Encoding::UTF_16_LE
                          : Encoding::UTF_16_BE)
        {}

        size_t getEncodedSize(const char32_t* src, size_t srcSize) override
        {
            size_t result = 0;
            for (size_t i = 0; i < srcSize; ++i)
            {
                if (src[i] <= 0xFFFF)
                    result +=  2;
                else if (src[i] <= UNICODE_MAX)
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
                auto n = encodeUtf16<SwapBytes>(src[i], cdst + bytesWritten, dstSize - bytesWritten);
                if (n == 0)
                    return {i, bytesWritten};
                bytesWritten += n;
            }
            return {srcSize, bytesWritten};
        }

        size_t encode(const char32_t* src, size_t srcSize,
                      std::string& dst) override
        {
            auto out = back_inserter(dst);
            for (size_t i = 0; i < srcSize; ++i)
                addUtf16Bytes<SwapBytes>(src[i], out);
            return srcSize;
        }
    };

    using Utf16BEEncoder = Utf16Encoder<IsLittleEndian>;
    using Utf16LEEncoder = Utf16Encoder<IsBigEndian>;
}
