//****************************************************************************
// Copyright © 2024 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2024-03-29.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <optional>
#include <string_view>
#include "Subrange.hpp"

namespace ystring
{
    [[nodiscard]]
    YSTRING_API Subrange find_first_escaped_char(std::string_view str,
                                                 size_t offset = 0);

    [[nodiscard]]
    YSTRING_API bool has_escaped_chars(std::string_view str);

    YSTRING_API size_t unescape(char str[], size_t length);

    [[nodiscard]]
    YSTRING_API std::string unescape(std::string str);

    /**
     * @brief Returns the next char in @a str, unescaped if it is escaped.
     *
     * If @a str is empty, the function returns std::nullopt.
     * The start of @a str is moved forward to the next character
     * if the function is successful.
     *
     * @param str A UTF-8 encoded string which may or may not contain escaped
     *  characters.
     * @param did_unescape If not nullptr, the function will set the value
     *  to true if the returned character was unescaped, and to false if it
     *  was not.
     * @return The next character.
     * @throw YstringException if the next character is an invalid UTF-8
     *  character or an invalid escape sequence.
     */
    [[nodiscard]]
    YSTRING_API std::optional<char32_t>
    unescape_next(std::string_view& str, bool* did_unescape = nullptr);
}
