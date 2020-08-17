//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-08-17.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <string>
#include <string_view>
#include "Ystring/YstringDefinitions.hpp"

namespace Ystring
{

    /** @brief Converts a UTF-32 (native endianness) string to UTF-8.
      */
    [[nodiscard]]
    YSTRING_API std::string fromUtf32(std::u32string_view str);

    /** @brief Converts a UTF-8 string to UTF-32 (native endianness).
      */
    YSTRING_API std::u32string toUtf32(std::string_view str);
}
