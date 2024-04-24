//****************************************************************************
// Copyright © 2024 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2024-04-20.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Char32Set.hpp"

namespace ystring
{
    void Char32Set::add_range(char32_t first, char32_t last)
    {
        ranges.emplace_back(first, last);
    }

    void Char32Set::add_char(char32_t ch)
    {
        if (!ranges.empty() && ranges.back().second + 1 == ch)
            ranges.back().second = ch;
        else
            add_range(ch, ch);
    }

    void Char32Set::add_chars(std::u32string_view chars)
    {
        for (char32_t ch : chars)
            add_char(ch);
    }

    bool Char32Set::contains(char32_t ch) const
    {
        return std::any_of(ranges.begin(), ranges.end(),
                           [&](auto& r) {return r.first <= ch && ch <= r.second;})
               != negated;
    }

    bool Char32Set::case_insensitive_contains(char32_t ch) const
    {
        return std::any_of(ranges.begin(), ranges.end(),
                           [&](auto& r)
                           {
                               if (r.first == r.second)
                                   return case_insensitive_equal(ch, r.first);
                               return !case_insensitive_less(ch, r.first)
                                      && !case_insensitive_less(r.second, ch);
                           }) != negated;
    }

    bool contains(std::u32string_view span, char32_t ch)
    {
        return std::find(span.begin(), span.end(), ch) != span.end();
    }

    bool case_insensitive_contains(std::u32string_view span, char32_t ch)
    {
        return std::find_if(
            span.begin(), span.end(),
            [&](auto c) {return case_insensitive_equal(ch, c);}) != span.end();
    }
}
