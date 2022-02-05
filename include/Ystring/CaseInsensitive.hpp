//***************************************************************************
// Copyright (C) 2011, Jan Erik Breimo.
// Created 2011-07-18 by Jan Erik Breimo
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//***************************************************************************
#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include "ConvertCase.hpp"

namespace ystring
{
    inline int32_t case_insensitive_compare(char32_t lhs, char32_t rhs)
    {
        return static_cast<int32_t>(to_upper(lhs) - to_upper(rhs));
    }

    inline bool case_insensitive_equal(char32_t lhs, char32_t rhs)
    {
        return to_upper(lhs) == to_upper(rhs);
    }

    inline bool case_insensitive_less(char32_t lhs, char32_t rhs)
    {
        return to_upper(lhs) < to_upper(rhs);
    }

    struct CaseInsensitiveCompare
    {
        int32_t operator()(char32_t a, char32_t b) const
        {
            return case_insensitive_compare(a, b);
        }
    };

    struct CaseInsensitiveEqual
    {
        bool operator()(char32_t a, char32_t b) const
        {
            return case_insensitive_equal(a, b);
        }
    };

    struct CaseInsensitiveLess
    {
        bool operator()(char32_t a, char32_t b) const
        {
            return case_insensitive_less(a, b);
        }
    };
}
