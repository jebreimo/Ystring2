//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-07-08.
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <cstdint>
#include <tuple>
#include "Ystring/Endian.hpp"
#include "Ystring/UnicodeChars.hpp"
//#include "DecoderResult.hpp"

namespace Ystring
{
    namespace Detail
    {
        template <bool SwapBytes, typename BiIt>
        char32_t nextWord(BiIt& it, BiIt end)
        {
            if (it == end)
                return INVALID;

            if constexpr (sizeof(*it) == 1)
            {
                union U
                {
                    char16_t c;
                    uint8_t b[2];
                } u;
                u.b[SwapBytes ? 1 : 0] = uint8_t(*it++);
                if (it == end)
                    return INVALID;
                u.b[SwapBytes ? 0 : 1] = uint8_t(*it++);
                return u.c;
            }
            else
            {
                return swapEndianness<SwapBytes>(char16_t(*it++));
            }
        }

        template <bool SwapBytes, typename BiIt>
        char32_t prevWord(BiIt begin, BiIt& it)
        {
            if (it == begin)
                return INVALID;

            if constexpr (sizeof(*it) == 1)
            {
                union U
                {
                    char16_t c;
                    uint8_t b[2];
                } u;
                if (--it == begin)
                    return INVALID;
                u.b[SwapBytes ? 0 : 1] = uint8_t(*it--);
                u.b[SwapBytes ? 1 : 0] = uint8_t(*it);
                return u.c;
            }
            else
            {
                return swapEndianness<SwapBytes>(char16_t(*--it));
            }
        }
    }

    template <bool SwapBytes, typename BiIt>
    char32_t nextUtf16CodePoint(BiIt& it, BiIt end)
    {
        auto first = it;
        auto chr = Detail::nextWord<SwapBytes>(it, end);
        if (chr == INVALID)
        {
            it = first;
            return INVALID;
        }

        if (chr < 0xD800 || 0xE000 <= chr)
            return chr;

        if (0xDC00 <= chr)
        {
            it = first;
            return INVALID;
        }

        auto chr2 = Detail::nextWord<SwapBytes>(it, end);
        if (chr2 == INVALID || chr2 < 0xDC00 || 0xE000 <= chr2)
        {
            it = first;
            return INVALID;
        }

        return char32_t(((chr & 0x3FFu) << 10u) + (chr2 & 0x3FFu) + 0x10000);
    }

    template <typename FwdIt>
    char32_t nextUtf16LECodePoint(FwdIt& it, FwdIt end)
    {
        return nextUtf16CodePoint<IsBigEndian>(it, end);
    }

    template <typename FwdIt>
    char32_t nextUtf16BECodePoint(FwdIt& it, FwdIt end)
    {
        return nextUtf16CodePoint<IsLittleEndian>(it, end);
    }

    template <bool SwapBytes, typename FwdIt>
    bool skipNextUtf16CodePoint(FwdIt& it, FwdIt end)
    {
        if (it == end)
            return false;
        auto chr = Detail::nextWord<SwapBytes>(it, end);
        if (chr < 0xD800 || 0xDC00 <= chr)
            return true;

        auto pos = it;
        auto chr2 = Detail::nextWord<SwapBytes>(it, end);
        if (chr2 != INVALID && (chr2 < 0xDC00 || 0xE000 <= chr2))
            it = pos;
        return true;
    }

    template <typename FwdIt>
    bool skipNextUtf16LECodePoint(FwdIt& it, FwdIt end)
    {
        return skipNextUtf16CodePoint<IsBigEndian>(it, end);
    }

    template <typename FwdIt>
    bool skipNextUtf16BECodePoint(FwdIt& it, FwdIt end)
    {
        return skipNextUtf16CodePoint<IsLittleEndian>(it, end);
    }

    template <bool SwapBytes, typename BiIt>
    char32_t prevUtf16CodePoint(BiIt begin, BiIt& it)
    {
        auto first = it;
        auto chr = Detail::prevWord<SwapBytes>(begin, it);
        if (chr == INVALID)
        {
            it = first;
            return INVALID;
        }

        if (chr < 0xD800 || 0xE000 <= chr)
            return chr;

        if (chr < 0xDC00)
        {
            it = first;
            return INVALID;
        }

        auto chr2 = Detail::prevWord<SwapBytes>(begin, it);
        if (chr2 == INVALID || chr2 < 0xD800 || 0xDC00 <= chr2)
        {
            it = first;
                return INVALID;
        }

        return char32_t(((chr2 & 0x3FFu) << 10u) + (chr & 0x3FFu) + 0x10000);
    }

    template <typename BiIt>
    char32_t prevUtf16LECodePoint(BiIt begin, BiIt& it)
    {
        return prevUtf16CodePoint<IsBigEndian>(begin, it);
    }

    template <typename BiIt>
    char32_t prevUtf16BECodePoint(BiIt begin, BiIt& it)
    {
        return prevUtf16CodePoint<IsLittleEndian>(begin, it);
    }

    template <bool SwapBytes, typename BiIt>
    bool skipPrevUtf16CodePoint(BiIt begin, BiIt& it)
    {
        if (it == begin)
            return false;

        auto chr = Detail::prevWord<SwapBytes>(begin, it);
        if (chr < 0xDC00 || 0xE000 <= chr)
            return true;

        auto next = it;
        auto chr2 = Detail::prevWord<SwapBytes>(begin, it);
        if (chr2 != INVALID && (chr2 < 0xD800 || 0xDC00 <= chr2))
            it = next;

        return true;
    }

    template <typename BiIt>
    bool skipPrevUtf16LECodePoint(BiIt begin, BiIt& it)
    {
        return skipPrevUtf16CodePoint<IsBigEndian>(begin, it);
    }

    template <typename BiIt>
    bool skipPrevUtf16BECodePoint(BiIt begin, BiIt& it)
    {
        return skipPrevUtf16CodePoint<IsLittleEndian>(begin, it);
    }
}
