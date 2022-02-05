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
#include "CodePointConstants.hpp"
#include "YstringException.hpp"

/** @file
  * @brief Defines functions for working with UTF-8 encoded strings.
  */

namespace ystring
{
    template <typename BiIt>
    char32_t decode_next(BiIt& it, BiIt end)
    {
        if (it == end)
            return INVALID_CHAR;

        if ((uint8_t(*it) & 0x80u) == 0)
            return uint8_t(*it++);

        char32_t result;
        int n = 0;
        if ((uint8_t(*it) & 0xE0u) == 0xC0)
        {
            result = uint8_t(*it++) & 0x1Fu;
            n = 1;
        }
        else if ((uint8_t(*it) & 0xF0u) == 0xE0)
        {
            result = uint8_t(*it++) & 0x0Fu;
            n = 2;
        }
        else if ((uint8_t(*it) & 0xF8u) == 0xF0)
        {
            result = uint8_t(*it++) & 0x07u;
            n = 3;
        }
        else
        {
            return INVALID_CHAR;
        }

        if (n > std::distance(it, end))
        {
            --it;
            return INVALID_CHAR;
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
                return INVALID_CHAR;
            }
            ++it;
        }

        return result;
    }

    template <typename BiIt>
    char32_t decode_prev(BiIt begin, BiIt& it)
    {
        if (it == begin)
            return INVALID_CHAR;

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
                return INVALID_CHAR;
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
        return INVALID_CHAR;
    }

    template <typename FwdIt>
    bool skip_next(FwdIt& it, FwdIt end)
    {
        if (it == end)
            return false;

        auto c = uint8_t(*it++);

        if ((c & 0x80u) == 0)
            return true;

        uint32_t n;
        if ((c & 0xE0u) == 0xC0)
            n = 1;
        else if ((c & 0xF0u) == 0xE0)
            n = 2;
        else if ((c & 0xF8u) == 0xF0)
            n = 3;
        else
            n = UINT32_MAX;

        while (it != end && (uint8_t(*it) & 0xC0u) == 0x80 && n-- > 0)
            ++it;

        return true;
    }

    template <typename BiIt>
    bool skip_prev(BiIt begin, BiIt& it)
    {
        if (it == begin)
            return false;

        uint32_t n = 0;
        while ((uint8_t(*(--it)) & 0xC0u) == 0x80)
        {
            if (it == begin)
                return true;
            ++n;
        }

        auto c = uint8_t(*it);
        uint32_t m;
        if ((c & 0x80u) == 0)
            m = 1;
        else if ((c & 0xE0u) == 0xC0)
            m = 2;
        else if ((c & 0xF0u) == 0xE0)
            m = 3;
        else if ((c & 0xF8u) == 0xF0)
            m = 4;
        else
            m = UINT32_MAX;

        if (m >= n + 1)
            return true;
        for (uint32_t i = 0; i < m; ++i)
            ++it;
        return true;
    }

    template <typename It>
    bool safe_decode_next(It& it, It end, char32_t& ch)
    {
        if (it == end)
            return false;
        ch = decode_next(it, end);
        if (ch == INVALID_CHAR)
            YSTRING_THROW("Invalid UTF-8 string.");
        return true;
    }

    template <typename It>
    bool safe_decode_prev(It begin, It& it, char32_t& ch)
    {
        if (begin == it)
            return false;
        ch = decode_prev(begin, it);
        if (ch == INVALID_CHAR)
            YSTRING_THROW("Invalid UTF-8 string.");
        return true;
    }
}
