//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-01-30.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <string_view>
#include "Subrange.hpp"

namespace ystring
{
    [[nodiscard]]
    YSTRING_API Subrange find_first_escapable_character(std::string_view str,
                                                        size_t offset = 0);

    [[nodiscard]]
    YSTRING_API Subrange find_first_escaped_character(std::string_view str,
                                                      size_t offset = 0);

    /**
     * @brief Returns true if @a str contains characters that will be escaped
     *      by escape_json(str).
     */
    [[nodiscard]]
    YSTRING_API bool has_escapable_characters(std::string_view str);

    [[nodiscard]]
    YSTRING_API bool has_escaped_characters(std::string_view str);

    [[nodiscard]]
    YSTRING_API std::string escape_json(std::string_view str);

    [[nodiscard]]
    YSTRING_API size_t unescape(char str[], size_t length);

    [[nodiscard]]
    YSTRING_API std::string unescape(std::string str);
}
