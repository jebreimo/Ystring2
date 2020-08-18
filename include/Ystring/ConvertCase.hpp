//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-07-29.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Ystring/YstringDefinitions.hpp"
#include <string>
#include <string_view>

namespace Ystring
{
    [[nodiscard]]
    YSTRING_API char32_t toLower(char32_t codePoint);

    [[nodiscard]]
    YSTRING_API char32_t toTitle(char32_t codePoint);

    [[nodiscard]]
    YSTRING_API char32_t toUpper(char32_t codePoint);

    /**
     * @brief Returns a lower case copy of @a str.
     */
    [[nodiscard]]
    YSTRING_API std::string toLower(std::string_view str);

    /**
     * @brief Returns a title-cased copy of @a str.
     */
    [[nodiscard]]
    YSTRING_API std::string toTitle(std::string_view str);

    /**
     * @brief Returns a upper case copy of @a str.
     */
    [[nodiscard]]
    YSTRING_API std::string toUpper(std::string_view str);
}
