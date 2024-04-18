//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-05-25.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <algorithm>
#include <string_view>
#include "CaseInsensitive.hpp"

namespace ystring
{
    [[nodiscard]]
    inline bool contains(std::u32string_view span,
                         char32_t ch)
    {
        return std::find(span.begin(), span.end(), ch) != span.end();
    }

    [[nodiscard]]
    inline bool case_insensitive_contains(std::u32string_view span,
                                          char32_t ch)
    {
        return std::find_if(
            span.begin(), span.end(),
            [&](auto c) {return case_insensitive_equal(ch, c);}) != span.end();
    }

}
