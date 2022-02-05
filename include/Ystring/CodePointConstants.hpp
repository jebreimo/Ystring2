//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-08-01.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

namespace ystring
{
    constexpr char32_t NEXT_LINE = 0x0085u;

    constexpr char32_t LINE_SEPARATOR = 0x2028u;

    constexpr char32_t PARAGRAPH_SEPARATOR = 0x2029u;

    constexpr char32_t REPLACEMENT_CHARACTER = 0xFFFDu;

    /** @brief The maxmum value for unicode code points.
      */
    constexpr char32_t UNICODE_MAX = 0x10FFFF;

    /** @brief A value representing invalid code points.
      */
    constexpr char32_t INVALID_CHAR = 0xFFFFFFFFu;
}
