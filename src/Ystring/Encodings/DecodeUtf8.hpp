//****************************************************************************
// Copyright © 2011, Jan Erik Breimo.
// Created 2011-07-14 by Jan Erik Breimo
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <cstdint>
#include <iterator>
#include <limits>
#include <tuple>
#include "Ystring/Unicode/UnicodeChars.hpp"

/** @file
  * @brief Defines functions for working with UTF-8 encoded strings.
  */

namespace Ystring
{
    namespace Detail
    {
        //static inline bool isContinuation(char c)
        //{
        //    return (uint8_t(c) & 0xC0u) == 0x80;
        //}

        //static inline bool isAscii(char32_t c)
        //{
        //    return (c & 0x80u) == 0;
        //}
        //
        //template <typename FwdIt>
        //FwdIt findEndOfCodePoint(FwdIt begin, FwdIt end)
        //{
        //    while (begin != end && isContinuation(*begin))
        //        ++begin;
        //    return begin;
        //}
    }

    template <typename BiIt>
    char32_t nextUtf8Value(BiIt& it, BiIt end)
    {
        if (it == end)
            return INVALID;

        if ((uint8_t(*it) & 0x80u) == 0)
            return uint8_t(*it++);

        char32_t result;
        int n = 0;
        if ((uint8_t(*it) & 0xE0u) == 0xC0)
        {
            result = *it++ & 0x1Fu;
            n = 1;
        }
        else if ((uint8_t(*it) & 0xF0u) == 0xE0)
        {
            result = *it++ & 0x0Fu;
            n = 2;
        }
        else if ((uint8_t(*it) & 0xF8u) == 0xF0)
        {
            result = *it++ & 0x07u;
            n = 3;
        }
        else
        {
            return INVALID;
        }

        for (int i = 0; i < n; ++i)
        {
            result <<= 6u;
            if (auto c = uint8_t(*it); (c & 0xC0u) == 0x80)
            {
                result |= c & 0x3Fu;
            }
            else
            {
                for (int j = 0; j <= i; ++j)
                    --it;
                return INVALID;
            }
            ++it;
        }

        return result;
    }

    template <typename BiIt>
    char32_t prevUtf8Value(BiIt begin, BiIt& it)
    {
        if (it == begin)
            return INVALID;

        --it;
        if ((uint8_t(*it) & 0x80u) == 0)
            return uint8_t(*it);

        char32_t result = 0;
        unsigned n = 0;
        while ((uint8_t(*it) & 0xC0u) == 0x80)
        {
            if (it == begin)
            {
                for (unsigned i = 0; i <= n; ++i)
                    ++it;
                return INVALID;
            }
            result |= (uint8_t(*it) & 0x3Fu) << (n * 6u);
            ++n;
            --it;
        }

        auto c = uint8_t(*it);
        if ((c & 0xE0u) == 0xC0 && n == 1)
            return result | ((c & 0x1Fu) << 6u);
        else if ((c & 0xF0u) == 0xE0 && n == 2)
            return result | ((c & 0x0Fu) << 12u);
        else if ((c & 0xF8u) == 0xF0 && n == 3)
            return result | ((c & 0x07u) << 18u);

        for (unsigned i = 0; i <= n; ++i)
            ++it;
        return INVALID;
    }
    //template <typename FwdIt>
    //bool skipNextUtf8Value(FwdIt& it, FwdIt end, size_t count = 1);
    //
    //template <typename BiIt>
    //bool skipPrevUtf8Value(BiIt begin, BiIt& it, size_t count = 1);
}

//#include "DecodeUtf8-impl.hpp"
