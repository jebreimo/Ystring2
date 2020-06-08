//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-07-08
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <cstdint>
#include <limits>
#include <string>
#include "Endian.hpp"

namespace Ystring
{
    namespace Detail
    {
        template <bool SwapBytes, typename OutIt>
        void addBytes(OutIt& begin, char16_t c)
        {
            union {char16_t c; char b[2];} u = {swapEndianness<SwapBytes>(c)};
            *begin++ = u.b[0];
            *begin++ = u.b[1];
        }

        template <bool SwapBytes, typename OutIt>
        void addWord(OutIt& begin, char16_t c)
        {
            *begin++ = swapEndianness<SwapBytes>(c);
        }
    }

    template <bool SwapBytes, typename OutIt>
    void addUtf16Words(OutIt out, char32_t codePoint)
    {
        if (codePoint <= 0xFFFF)
        {
            Detail::addWord<SwapBytes>(out, char16_t(codePoint));
        }
        else
        {
            codePoint -= 0x10000;
            auto word1 = char16_t(0xD800u | (codePoint >> 10u));
            Detail::addWord<SwapBytes>(out, word1);
            auto word2 = char16_t(0xDC00u | (codePoint & 0x3FFu));
            Detail::addWord<SwapBytes>(out, word2);
        }
    }

    template <typename OutIt>
    void addUtf16LEWords(OutIt out, char32_t codePoint)
    {
        return addUtf16Words<IsBigEndian>(out, codePoint);
    }

    template <typename OutIt>
    void addUtf16BEWords(OutIt out, char32_t codePoint)
    {
        return addUtf16Words<IsLittleEndian>(out, codePoint);
    }

    template <bool SwapBytes, typename OutIt>
    void addUtf16Bytes(OutIt out, char32_t codePoint)
    {
        if (codePoint <= 0xFFFF)
        {
            Detail::addBytes<SwapBytes>(out, char16_t(codePoint));
        }
        else
        {
            codePoint -= 0x10000;
            auto word1 = char16_t(0xD800u | (codePoint >> 10u));
            auto word2 = char16_t(0xDC00u | (codePoint & 0x3FFu));
            Detail::addBytes<SwapBytes>(out, word1);
            Detail::addBytes<SwapBytes>(out, word2);
        }
    }

    template <typename OutIt>
    void addUtf16LEBytes(OutIt out, char32_t codePoint)
    {
        return addUtf16Bytes<IsBigEndian>(out, codePoint);
    }

    template <typename OutIt>
    void addUtf16BEBytes(OutIt out, char32_t codePoint)
    {
        return addUtf16Bytes<IsLittleEndian>(out, codePoint);
    }

    template <bool SwapBytes, typename T>
    size_t encodeUtf16(T* data, size_t n, char32_t codePoint)
    {
        using CharType = typename std::decay<decltype(*data)>::type;
        if (codePoint <= 0xFFFF)
        {
            if constexpr (sizeof(T) == 1)
            {
                if (n < 2)
                    return 0;
                Detail::addBytes<SwapBytes>(data, char16_t(codePoint));
                return 2;
            }
            else
            {
                if (n == 0)
                    return 0;
                Detail::addWord<SwapBytes>(data, char16_t(codePoint));
                return 1;
            }
        }
        else
        {
            codePoint -= 0x10000;
            auto word1 = char16_t(0xD800u | (codePoint >> 10u));
            auto word2 = char16_t(0xDC00u | (codePoint & 0x3FFu));
            if constexpr (sizeof(T) == 1)
            {
                if (n < 4)
                    return 0;
                Detail::addBytes<SwapBytes>(data, word1);
                Detail::addBytes<SwapBytes>(data, word2);
                return 4;
            }
            else
            {
                if (n < 2)
                    return 0;
                Detail::addWord<SwapBytes>(data, word1);
                Detail::addWord<SwapBytes>(data, word2);
                return 2;
            }
        }
    }

    template <typename T>
    size_t encodeUtf16LE(T* data, size_t n, char32_t codePoint)
    {
        return encodeUtf16<IsBigEndian>(data, n, codePoint);
    }

    template <typename T>
    size_t encodeUtf16BE(T* data, size_t n, char32_t codePoint)
    {
        return encodeUtf16<IsLittleEndian>(data, n, codePoint);
    }
}
