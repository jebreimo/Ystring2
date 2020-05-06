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
#include "CaseConverter.hpp"

namespace Ystring { namespace Unicode
{
    inline int32_t caseInsensitiveCompare(char32_t lhs, char32_t rhs)
    {
        return static_cast<int32_t>(upper(lhs) - upper(rhs));
    }

    inline bool caseInsensitiveEqual(char32_t lhs, char32_t rhs)
    {
        return upper(lhs) == upper(rhs);
    }

    inline bool caseInsensitiveLess(char32_t lhs, char32_t rhs)
    {
        return upper(lhs) < upper(rhs);
    }

    struct CaseInsensitiveCompare
        : std::binary_function<char32_t, char32_t, int32_t>
    {
        int32_t operator()(char32_t a, char32_t b) const
        {
            return caseInsensitiveCompare(a, b);
        }
    };

    struct CaseInsensitiveEqual
        : std::binary_function<char32_t, char32_t, bool>
    {
        bool operator()(char32_t a, char32_t b) const
        {
            return caseInsensitiveEqual(a, b);
        }
    };

    struct CaseInsensitiveLess
        : std::binary_function<char32_t, char32_t, bool>
    {
        bool operator()(char32_t a, char32_t b) const
        {
            return caseInsensitiveLess(a, b);
        }
    };
}}
