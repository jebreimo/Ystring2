//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-05-08.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "Ystring/CharClass.hpp"
#include "Ystring/CodePointConstants.hpp"

/** @file
  * @brief Defines predicates for testing properties of unicode code-points.
  */

namespace Ystring
{
    /** @brief Returns true if @a c is a letter or a number.
      */
    [[nodiscard]]
    inline bool isAlphaNumeric(char32_t c)
    {
        return (getCharClass(c) & (CharClass::LETTER | CharClass::NUMBER)) != 0;
    }

    /** @brief Returns true if @a c is a letter that is lowercased, uppercased
      *     titlecased.
      */
    [[nodiscard]]
    inline bool isCasedLetter(char32_t c)
    {
        return (getCharClass(c) & CharClass::CASED_LETTER) != 0;
    }

    /** @brief Returns true if @a c is one of the control characters.
      */
    [[nodiscard]]
    inline bool isControl(char32_t c)
    {
        return (getCharClass(c) & CharClass::CONTROL) != 0;
    }

    /** @brief Returns true if @a c is one of the formatting characters.
      */
    [[nodiscard]]
    inline bool isFormat(char32_t c)
    {
        return (getCharClass(c) & CharClass::FORMAT) != 0;
    }

    /** @brief Returns true if @a c is a character that can be drawn
      *     in any way.
      */
    [[nodiscard]]
    inline bool isGraphical(char32_t c)
    {
        return (getCharClass(c) & (CharClass::LETTER |
                                   CharClass::MARK |
                                   CharClass::NUMBER |
                                   CharClass::PUNCTUATION |
                                   CharClass::SYMBOL)) != 0;
    }

    /** @brief Returns true if @a c is among the hexadecimal
      *     digits (0-9 and A-F).
      */
    [[nodiscard]]
    inline bool isHexDigit(char32_t c)
    {
        return ('0' <= c && c <= '9') ||
               ('A' <= c && c <= 'F') ||
               ('a' <= c && c <= 'f');
    }

    /** @brief Returns true if @a c is a letter.
      */
    [[nodiscard]]
    inline bool isLetter(char32_t c)
    {
        return (getCharClass(c) & CharClass::LETTER) != 0;
    }

    /** @brief Returns true if @a c is a combining mark.
      */
    [[nodiscard]]
    inline bool isMark(char32_t c)
    {
        return (getCharClass(c) & CharClass::MARK) != 0;
    }

    /** @brief Returns true if @a c is among the characters used to represent
      * newlines.
      *
      * @note Windows (and MSDOS) uses the sequence of "\\r\\n" to represent
      *     newlines and isNewLine will return true for both of these.
      */
    [[nodiscard]]
    inline bool isNewline(char32_t c)
    {
        return c == '\n' || c == '\v' || c == '\f' || c == '\r' ||
               c == NEXT_LINE || c == LINE_SEPARATOR ||
               c == PARAGRAPH_SEPARATOR;
    }

    /** @brief Returns true if @a c is a number.
      *
      * @note Unicode supports numbers from all written languages, not
      *     just the western numbers 0-9.
      */
    [[nodiscard]]
    inline bool isNumber(char32_t c)
    {
        return (getCharClass(c) & CharClass::NUMBER) != 0;
    }

    /** @brief Returns true if @a c is a punctuation symbol.
      */
    [[nodiscard]]
    inline bool isPunctuation(char32_t c)
    {
        return (getCharClass(c) & CharClass::PUNCTUATION) != 0;
    }

    /** @brief Returns true if @a c is a separator of one kind or another.
      */
    [[nodiscard]]
    inline bool isSeparator(char32_t c)
    {
        return (getCharClass(c) & CharClass::SEPARATOR) != 0;
    }

    /** @brief Returns true if @a c is one of the values used to compose
      *     surrogate pairs.
      */
    [[nodiscard]]
    inline bool isSurrogate(char32_t c)
    {
        return (getCharClass(c) & CharClass::SURROGATE) != 0;
    }

    /** @brief Returns true if @a c is one of the unicode symbols.
      */
    [[nodiscard]]
    inline bool isSymbol(char32_t c)
    {
        return (getCharClass(c) & CharClass::SYMBOL) != 0;
    }

    /** @brief Returns true if @a c is a visible character.
      *
      * Visible characters are the sum of graphical characters and separators.
      */
    [[nodiscard]]
    inline bool isVisible(char32_t c)
    {
        return (getCharClass(c) & (CharClass::LETTER |
                                   CharClass::MARK |
                                   CharClass::NUMBER |
                                   CharClass::PUNCTUATION |
                                   CharClass::SYMBOL |
                                   CharClass::SEPARATOR)) != 0;
    }

    /** @brief Returns true if @a c is a whitespace character.
      *
      * The whitespace character class include tab, newlines, space and all
      * separator characters.
      */
    [[nodiscard]]
    inline bool isWhitespace(char32_t c)
    {
        return c == '\t' || c == ' ' || isNewline(c) ||
               (c > 128 && (getCharClass(c) & CharClass::SEPARATOR) != 0);
    }
}
