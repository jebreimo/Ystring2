//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-07-29
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/CharClass.hpp"

#include <algorithm>
#include <iterator>
#include "CharClassTables.hpp"

namespace Ystring
{
    namespace
    {
        constexpr unsigned CODE_POINT_SHIFT = 11;

        unsigned decodeCharClass(char32_t codePoint, char32_t encodedCharClass)
        {
            auto lowerCharClass = encodedCharClass & 0x1Fu;
            auto upperCharClass = (encodedCharClass >> 5u) & 0x3Fu;
            if (upperCharClass == 0)
                return lowerCharClass;
            auto endCodePoint = encodedCharClass >> CODE_POINT_SHIFT;
            if (upperCharClass == 0x20)
            {
                if (((codePoint ^ endCodePoint) & 1u) != 0)
                    return 1 - lowerCharClass;
                return lowerCharClass;
            }
            if (codePoint == endCodePoint)
                return lowerCharClass;
            return upperCharClass;
        }
    }

    CharClass_t getCharClass(char32_t codePoint)
    {
        if (codePoint < 128)
            return CharClass_t(1u << ASCII_CHAR_CLASSES[codePoint]);
        char32_t key = codePoint << CODE_POINT_SHIFT;
        auto it = std::lower_bound(
            std::begin(COMPLETE_CHAR_CLASSES),
            std::end(COMPLETE_CHAR_CLASSES),
            key);
        if (it == std::end(COMPLETE_CHAR_CLASSES))
            return CharClass::UNASSIGNED;
        return CharClass_t(1u << decodeCharClass(codePoint, *it));
    }
}
