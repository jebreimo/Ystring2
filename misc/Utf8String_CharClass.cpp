//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-05-09.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Utf8String_CharClass.hpp"

#include "Ystring/CodePointPredicates.hpp"
#include "Ystring/DecodeUtf8.hpp"

namespace Ystring
{
    size_t countCharacters(std::string_view str)
    {
        auto it = str.begin();
        auto end = str.end();
        size_t result = 0;
        char32_t ch;
        while (safeNextUtf8Value(it, end, ch))
        {
            if (!isMark(ch))
                ++result;
        }
        return result;
    }
}
