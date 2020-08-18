//****************************************************************************
// Copyright © 2013 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2013-07-01
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <cassert>
#include <cstdint>
#include <optional>
#include "Ystring/YstringDefinitions.hpp"

namespace Ystring
{
    struct YSTRING_API CompactCharMapping
    {
        char32_t segment;
        char32_t offset;
        char32_t affected;
        char32_t ignorable;

        [[nodiscard]]
        std::optional<char32_t> get(char32_t codePoint) const
        {
            assert(codePoint - segment < 32);
            auto mask = char32_t(1) << (codePoint - segment);
            if (mask & affected)
                return codePoint + offset;
            if (mask & ignorable)
                return codePoint;
            return {};
        }
    };

    struct CharMapping
    {
        char32_t fromCodePoint;
        char32_t toCodePoint;
    };
}
