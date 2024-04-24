//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-05-25.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <algorithm>
#include <vector>
#include <string_view>
#include "CaseInsensitive.hpp"

namespace ystring
{
    using CharRange = std::pair<char32_t, char32_t>;

    struct Char32Set
    {
        std::vector<CharRange> ranges;
        bool negated = false;

        void add_range(char32_t first, char32_t last);

        void add_char(char32_t ch);

        void add_chars(std::u32string_view chars);

        [[nodiscard]]
        bool contains(char32_t ch) const;

        [[nodiscard]]
        bool case_insensitive_contains(char32_t ch) const;
    };

    [[nodiscard]]
    bool contains(std::u32string_view span, char32_t ch);

    [[nodiscard]]
    bool case_insensitive_contains(std::u32string_view span, char32_t ch);

}
