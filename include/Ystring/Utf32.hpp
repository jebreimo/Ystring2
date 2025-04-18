//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-08-17.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <string>
#include <string_view>
#include "Ystring/YstringDefinitions.hpp"

namespace ystring
{
    /** @brief Converts a UTF-32 (native endianness) character to UTF-8.
      */
    [[nodiscard]]
    YSTRING_API std::string from_utf32(char32_t ch);

    /** @brief Converts a UTF-32 (native endianness) string to UTF-8.
      */
    [[nodiscard]]
    YSTRING_API std::string from_utf32(std::u32string_view str);

    /** @brief Converts a UTF-8 string to UTF-32 (native endianness).
      */
    YSTRING_API std::u32string to_utf32(std::string_view str);
}
