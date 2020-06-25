//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-06-25.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "DecoderBase.hpp"
#include "Ystring/DecodeUtf16.hpp"

namespace Ystring
{
    template <bool SwapBytes>
    class Utf16Decoder : public DecoderBase
    {
    public:
        Utf16Decoder() : DecoderBase(IsBigEndian == SwapBytes
                                     ? Encoding::UTF_16_LE
                                     : Encoding::UTF_16_BE)
        {}
    protected:
        size_t skipCharacter(const void* src, size_t srcSize) const final
        {
            auto cSrc = static_cast<const char*>(src);
            auto initialSrc = cSrc;
            skipNextUtf16CodePoint<SwapBytes>(cSrc, cSrc + srcSize);
            return size_t(cSrc - initialSrc);
        }

        std::pair<size_t, size_t>
        doDecode(const void* src, size_t srcSize,
                 char32_t* dst, size_t dstSize) const final
        {
            auto cSrc = static_cast<const char*>(src);
            auto initialSrc = cSrc;
            auto initialDst = dst;
            auto srcEnd = cSrc + srcSize;
            auto dstEnd = dst + dstSize;
            while (dst != dstEnd)
            {
                auto value = nextUtf16CodePoint<SwapBytes>(cSrc, srcEnd);
                if (value == INVALID)
                    break;
                *dst++ = value;
            }
            return {size_t(cSrc - initialSrc), size_t(dst - initialDst)};
        }
    };
}
