//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-05-09.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Utf8String.hpp"

namespace Ystring
{
    /** @brief Returns the number of characters in @a str.
      *
      * @note A composed character can consist of multiple code points.
      * @return the number of characters.
      * @throw YstringException if str contains an invalid UTF-8 code point.
      */
    YSTRING_API size_t countCharacters(std::string_view str);

    YSTRING_API Subrange getCharacter(std::string_view str, ptrdiff_t pos);

    /** @brief Returns the offset to the start of character number @a n
      *     in @a str.
      * @param n The number of complete characters (i.e. not code
      *     points if the string has combining marks) from the
      *     start of the string. If @a pos is negative it's from the end of
      *     the string instead.
      */
    YSTRING_API size_t getCharacterPosition(std::string_view str,
                                            ptrdiff_t pos);
}
