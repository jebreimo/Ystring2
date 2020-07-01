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
#include "UnicodeChars.hpp"

namespace Ystring
{
    namespace Detail
    {
        template <bool SwapBytes, typename OutIt>
        void addBytes(char16_t c, OutIt& begin)
        {
            union {char16_t c; char b[2];} u = {swapEndianness<SwapBytes>(c)};
            *begin++ = u.b[0];
            *begin++ = u.b[1];
        }

        template <bool SwapBytes, typename OutIt>
        void addWord(char16_t c, OutIt& begin)
        {
            *begin++ = swapEndianness<SwapBytes>(c);
        }
    }

    template <bool SwapBytes, typename OutIt>
    void addUtf16Words(char32_t codePoint, OutIt out)
    {
        if (codePoint <= 0xFFFF)
        {
            Detail::addWord<SwapBytes>(char16_t(codePoint), out);
        }
        else if (codePoint <= UNICODE_MAX)
        {
            codePoint -= 0x10000;
            auto word1 = char16_t(0xD800u | (codePoint >> 10u));
            Detail::addWord<SwapBytes>(word1, out);
            auto word2 = char16_t(0xDC00u | (codePoint & 0x3FFu));
            Detail::addWord<SwapBytes>(word2, out);
        }
    }

    template <typename OutIt>
    void addUtf16LEWords(char32_t codePoint, OutIt out)
    {
        return addUtf16Words<IsBigEndian>(codePoint, out);
    }

    template <typename OutIt>
    void addUtf16BEWords(char32_t codePoint, OutIt out)
    {
        return addUtf16Words<IsLittleEndian>(codePoint, out);
    }

    template <bool SwapBytes, typename OutIt>
    void addUtf16Bytes(char32_t codePoint, OutIt out)
    {
        if (codePoint <= 0xFFFF)
        {
            Detail::addBytes<SwapBytes>(char16_t(codePoint), out);
        }
        else if (codePoint <= UNICODE_MAX)
        {
            codePoint -= 0x10000;
            auto word1 = char16_t(0xD800u | (codePoint >> 10u));
            auto word2 = char16_t(0xDC00u | (codePoint & 0x3FFu));
            Detail::addBytes<SwapBytes>(word1, out);
            Detail::addBytes<SwapBytes>(word2, out);
        }
    }

    template <typename OutIt>
    void addUtf16LEBytes(char32_t codePoint, OutIt out)
    {
        return addUtf16Bytes<IsBigEndian>(codePoint, out);
    }

    template <typename OutIt>
    void addUtf16BEBytes(char32_t codePoint, OutIt out)
    {
        return addUtf16Bytes<IsLittleEndian>(codePoint, out);
    }

    template <bool SwapBytes, typename T>
    size_t encodeUtf16(char32_t codePoint, T* data, size_t n)
    {
        using CharType = typename std::decay<decltype(*data)>::type;
        if (codePoint <= 0xFFFF)
        {
            if constexpr (sizeof(T) == 1)
            {
                if (n < 2)
                    return 0;
                Detail::addBytes<SwapBytes>(char16_t(codePoint), data);
                return 2;
            }
            else if (codePoint <= UNICODE_MAX)
            {
                if (n == 0)
                    return 0;
                Detail::addWord<SwapBytes>(char16_t(codePoint), data);
                return 1;
            }
        }
        else if (codePoint <= UNICODE_MAX)
        {
            codePoint -= 0x10000;
            auto word1 = char16_t(0xD800u | (codePoint >> 10u));
            auto word2 = char16_t(0xDC00u | (codePoint & 0x3FFu));
            if constexpr (sizeof(T) == 1)
            {
                if (n < 4)
                    return 0;
                Detail::addBytes<SwapBytes>(word1, data);
                Detail::addBytes<SwapBytes>(word2, data);
                return 4;
            }
            else
            {
                if (n < 2)
                    return 0;
                Detail::addWord<SwapBytes>(word1, data);
                Detail::addWord<SwapBytes>(word2, data);
                return 2;
            }
        }
        return 0;
    }

    template <typename T>
    size_t encodeUtf16LE(char32_t codePoint, T* data, size_t n)
    {
        return encodeUtf16<IsBigEndian>(codePoint, data, n);
    }

    template <typename T>
    size_t encodeUtf16BE(char32_t codePoint, T* data, size_t n)
    {
        return encodeUtf16<IsLittleEndian>(codePoint, data, n);
    }
}
