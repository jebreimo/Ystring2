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

// What are the values in the COMPLETE_CHAR_CLASSES table?
//
// They are actually a bit field struct:
//
// struct CHAR_CLASS
// {
//     uint32_t finalCodePoint : 21; // MSB
//     uint32_t isAlternatingCodePoints: 1;
//     uint32_t upperCharClass : 5;
//     uint32_t lowerCharClass : 5;
// };

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
                return lowerCharClass - 1;
            auto endCodePoint = encodedCharClass >> CODE_POINT_SHIFT;
            if (upperCharClass == 0x20)
            {
                if (((codePoint ^ endCodePoint) & 1u) != 0)
                    return 1 - (lowerCharClass - 1);
                return lowerCharClass - 1;
            }
            if (codePoint == endCodePoint)
                return lowerCharClass - 1;
            return upperCharClass - 1;
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
