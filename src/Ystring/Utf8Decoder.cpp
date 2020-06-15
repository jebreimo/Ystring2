//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-10-26
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Utf8Decoder.hpp"
#include "Ystring/DecodeUtf8.hpp"

namespace Ystring
{
    Utf8Decoder::Utf8Decoder()
        : DecoderBase(Encoding::UTF8)
    {}

    size_t Utf8Decoder::skipCharacter(const char* src, size_t srcSize) const
    {
        auto initialSrc = src;
        skipNextUtf8Value(src, src + srcSize);
        return size_t(src - initialSrc);
    }

    std::pair<size_t, size_t>
    Utf8Decoder::doDecode(const char* src, size_t srcSize,
                          char32_t* dst, size_t dstSize) const
    {
        auto initialSrc = src;
        auto initialDst = dst;
        auto srcEnd = src + srcSize;
        auto dstEnd = dst + dstSize;
        while (dst != dstEnd)
        {
            auto value = nextUtf8Value(src, srcEnd);
            if (value == INVALID)
                break;
            *dst++ = value;
        }
        return {size_t(src - initialSrc), size_t(dst - initialDst)};
    }
}
