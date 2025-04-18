//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-07-29.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Ystring/YstringDefinitions.hpp"
#include <string>
#include <string_view>

namespace ystring
{
    [[nodiscard]]
    YSTRING_API char32_t to_lower(char32_t codepoint);

    [[nodiscard]]
    YSTRING_API char32_t to_title(char32_t codepoint);

    [[nodiscard]]
    YSTRING_API char32_t to_upper(char32_t codepoint);

    /**
     * @brief Returns a lower case copy of @a str.
     */
    [[nodiscard]]
    YSTRING_API std::string to_lower(std::string_view str);

    /**
     * @brief Returns a title-cased copy of @a str.
     */
    [[nodiscard]]
    YSTRING_API std::string to_title(std::string_view str);

    /**
     * @brief Returns a upper case copy of @a str.
     */
    [[nodiscard]]
    YSTRING_API std::string to_upper(std::string_view str);
}
