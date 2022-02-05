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

namespace ystring
{
    /** @brief Returns true if @a c is a letter or a number.
      */
    [[nodiscard]]
    inline bool is_alpha_numeric(char32_t c)
    {
        return (get_char_class(c) & (CharClass::LETTER | CharClass::NUMBER)) != 0;
    }

    /** @brief Returns true if @a c is a letter that is lowercased, uppercased
      *     titlecased.
      */
    [[nodiscard]]
    inline bool is_cased_letter(char32_t c)
    {
        return (get_char_class(c) & CharClass::CASED_LETTER) != 0;
    }

    /** @brief Returns true if @a c is one of the control characters.
      */
    [[nodiscard]]
    inline bool is_control(char32_t c)
    {
        return (get_char_class(c) & CharClass::CONTROL) != 0;
    }

    /** @brief Returns true if @a c is one of the formatting characters.
      */
    [[nodiscard]]
    inline bool is_format(char32_t c)
    {
        return (get_char_class(c) & CharClass::FORMAT) != 0;
    }

    /** @brief Returns true if @a c is a character that can be drawn
      *     in any way.
      */
    [[nodiscard]]
    inline bool is_graphical(char32_t c)
    {
        return (get_char_class(c) & (CharClass::LETTER |
                                     CharClass::MARK |
                                     CharClass::NUMBER |
                                     CharClass::PUNCTUATION |
                                     CharClass::SYMBOL)) != 0;
    }

    /** @brief Returns true if @a c is among the hexadecimal
      *     digits (0-9 and A-F).
      */
    [[nodiscard]]
    inline bool is_hex_digit(char32_t c)
    {
        return ('0' <= c && c <= '9') ||
               ('A' <= c && c <= 'F') ||
               ('a' <= c && c <= 'f');
    }

    /** @brief Returns true if @a c is a letter.
      */
    [[nodiscard]]
    inline bool is_letter(char32_t c)
    {
        return (get_char_class(c) & CharClass::LETTER) != 0;
    }

    /** @brief Returns true if @a c is a lower case letter.
      */
    [[nodiscard]]
    inline bool is_lower(char32_t c)
    {
        return (get_char_class(c) & CharClass::LOWERCASE_LETTER) != 0;
    }

    /** @brief Returns true if @a c is a combining mark.
      */
    [[nodiscard]]
    inline bool is_mark(char32_t c)
    {
        return (get_char_class(c) & CharClass::MARK) != 0;
    }

    /** @brief Returns true if @a c is among the characters used to represent
      * newlines.
      *
      * @note Windows (and MSDOS) uses the sequence of "\\r\\n" to represent
      *     newlines and isNewLine will return true for both of these.
      */
    [[nodiscard]]
    inline bool is_newline(char32_t c)
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
    inline bool is_number(char32_t c)
    {
        return (get_char_class(c) & CharClass::NUMBER) != 0;
    }

    /** @brief Returns true if @a c is a punctuation symbol.
      */
    [[nodiscard]]
    inline bool is_punctuation(char32_t c)
    {
        return (get_char_class(c) & CharClass::PUNCTUATION) != 0;
    }

    /** @brief Returns true if @a c is a separator of one kind or another.
      */
    [[nodiscard]]
    inline bool is_separator(char32_t c)
    {
        return (get_char_class(c) & CharClass::SEPARATOR) != 0;
    }

    /** @brief Returns true if @a c is one of the values used to compose
      *     surrogate pairs.
      */
    [[nodiscard]]
    inline bool is_surrogate(char32_t c)
    {
        return (get_char_class(c) & CharClass::SURROGATE) != 0;
    }

    /** @brief Returns true if @a c is one of the unicode symbols.
      */
    [[nodiscard]]
    inline bool is_symbol(char32_t c)
    {
        return (get_char_class(c) & CharClass::SYMBOL) != 0;
    }

    /** @brief Returns true if @a c is a upper case letter.
      */
    [[nodiscard]]
    inline bool is_upper(char32_t c)
    {
        return (get_char_class(c) & CharClass::UPPERCASE_LETTER) != 0;
    }

    /** @brief Returns true if @a c is a visible character.
      *
      * Visible characters are the sum of graphical characters and separators.
      */
    [[nodiscard]]
    inline bool is_visible(char32_t c)
    {
        return (get_char_class(c) & (CharClass::LETTER |
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
    inline bool is_whitespace(char32_t c)
    {
        return c == '\t' || c == ' ' || is_newline(c) ||
               (c > 128 && (get_char_class(c) & CharClass::SEPARATOR) != 0);
    }
}
