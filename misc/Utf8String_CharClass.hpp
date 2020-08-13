//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-05-09.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Ystring/Algorithms.hpp"

namespace Ystring
{

    YSTRING_API Subrange getCharacterRange(std::string_view str, ptrdiff_t pos);

    /** @brief Returns the offset to the start of character number @a n
      *     in @a str.
      * @param n The number of complete characters (i.e. not code
      *     points if the string has combining marks) from the
      *     start of the string. If @a pos is negative it's from the end of
      *     the string instead.
      */
    YSTRING_API size_t getCharacterPosition(std::string_view str,
                                            ptrdiff_t pos);

    /** @brief Inserts string @a sub into @a str at position @a pos.
      *
      * @param pos The insert position in complete characters (i.e. not bytes,
      *     not even code points if the string has decomposed characters) from
      *     the start of the string. If @a pos is negative it's from the end
      *     of the string instead.
      * @throws YstringException if @a str isn't a valid UTF-8 string.
      */
    YSTRING_API std::string insert(
        std::string_view str,
        ptrdiff_t pos,
        std::string_view sub);

    /** @brief Inserts character @a chr into @a str at position @a pos.
      *
      * @param pos The insert position in complete characters (i.e. not bytes,
      *     not even code points if the string has decomposed characters)
      *     from the start of the string. If @a pos is negative it's from the
      *     end of the string instead.
      * @throws YstringException if @a str isn't a valid UTF-8 string.
      */
    YSTRING_API std::string insert(
        std::string_view str,
        ptrdiff_t pos,
        char32_t chr);
}
