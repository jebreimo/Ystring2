//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-05-07.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cstdint>
#include <iosfwd>
#include <iterator>
#include "YstringDefinitions.hpp"

namespace ystring
{
    struct Subrange
    {
        Subrange() = default;

        Subrange(size_t offset, size_t length)
            : offset(offset), length(length)
        {}

        Subrange(size_t offset)
            : offset(offset)
        {}

        template <typename It>
        Subrange(It begin, It subBegin, It subEnd)
            : offset(std::distance(begin, subBegin)),
              length(std::distance(subBegin, subEnd))
        {}

        [[nodiscard]] constexpr size_t start() const
        {
            return offset;
        }

        [[nodiscard]] constexpr size_t end() const
        {
            return offset + length;
        }

        constexpr explicit operator bool() const
        {
            return length != 0;
        }

        size_t offset = SIZE_MAX;
        size_t length = 0;
    };

    constexpr bool operator==(const Subrange& a, const Subrange& b)
    {
        return a.offset == b.offset && a.length == b.length;
    }

    constexpr bool operator!=(const Subrange& a, const Subrange& b)
    {
        return !(a == b);
    }

    YSTRING_API std::ostream& operator<<(std::ostream& os, const Subrange& s);
}
