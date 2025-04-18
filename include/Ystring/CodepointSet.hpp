//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-05-25.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <algorithm>
#include <iosfwd>
#include <string_view>
#include <vector>
#include "CaseInsensitive.hpp"

namespace ystring
{
    using CodepointRange = std::pair<char32_t, char32_t>;

    struct CodepointSet
    {
        std::vector<CodepointRange> ranges;
        bool negated = false;

        void add_range(char32_t first, char32_t last);

        void add_codepoint(char32_t cp);

        void add_codepoints(std::u32string_view codepoints);

        [[nodiscard]]
        bool contains(char32_t cp) const;

        [[nodiscard]]
        bool case_insensitive_contains(char32_t cp) const;
    };

    std::ostream& operator<<(std::ostream& os, const CodepointSet& set);

    [[nodiscard]]
    bool contains(std::u32string_view span, char32_t cp);

    [[nodiscard]]
    bool case_insensitive_contains(std::u32string_view span, char32_t cp);
}
