//****************************************************************************
// Copyright © 2024 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2024-04-20.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/CodepointSet.hpp"
#include "Ystring/Utf32.hpp"

#include <ostream>
#include <Ystring/Algorithms.hpp>

namespace ystring
{
    void CodepointSet::add_range(char32_t first, char32_t last)
    {
        ranges.emplace_back(first, last);
    }

    void CodepointSet::add_codepoint(char32_t cp)
    {
        if (!ranges.empty() && ranges.back().second + 1 == cp)
            ranges.back().second = cp;
        else
            add_range(cp, cp);
    }

    void CodepointSet::add_codepoints(std::u32string_view codepoints)
    {
        for (char32_t ch : codepoints)
            add_codepoint(ch);
    }

    bool CodepointSet::contains(char32_t cp) const
    {
        return std::any_of(ranges.begin(), ranges.end(),
                           [&](auto& r) {return r.first <= cp && cp <= r.second;})
               != negated;
    }

    bool CodepointSet::case_insensitive_contains(char32_t cp) const
    {
        return std::any_of(ranges.begin(), ranges.end(),
                           [&](auto& r)
                           {
                               if (r.first == r.second)
                                   return case_insensitive_equal(cp, r.first);
                               return !case_insensitive_less(cp, r.first)
                                      && !case_insensitive_less(r.second, cp);
                           }) != negated;
    }

    std::ostream& operator<<(std::ostream& os, const CodepointSet& set)
    {
        os << (set.negated ? "[^" : "[");
        for (auto& r : set.ranges)
        {
            if (r.first == r.second)
                os << from_utf32(r.first);
            else
                os << from_utf32(r.first) << '-' << from_utf32(r.second);
        }
        return os << ']';
    }

    bool contains(std::u32string_view span, char32_t cp)
    {
        return std::find(span.begin(), span.end(), cp) != span.end();
    }

    bool case_insensitive_contains(std::u32string_view span, char32_t cp)
    {
        return std::find_if(
            span.begin(), span.end(),
            [&](auto c) {return case_insensitive_equal(cp, c);}) != span.end();
    }
}
