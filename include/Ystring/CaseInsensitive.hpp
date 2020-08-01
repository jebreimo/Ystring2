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

namespace Ystring
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
    {
        int32_t operator()(char32_t a, char32_t b) const
        {
            return caseInsensitiveCompare(a, b);
        }
    };

    struct CaseInsensitiveEqual
    {
        bool operator()(char32_t a, char32_t b) const
        {
            return caseInsensitiveEqual(a, b);
        }
    };

    struct CaseInsensitiveLess
    {
        bool operator()(char32_t a, char32_t b) const
        {
            return caseInsensitiveLess(a, b);
        }
    };
}
