//****************************************************************************
// Copyright © 2013 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2013-07-01
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <cassert>
#include <cstdint>
#include <optional>
#include "Ystring/YstringDefinitions.hpp"

namespace ystring
{
    struct YSTRING_API CompactCharMapping
    {
        char32_t segment;
        char32_t offset;
        char32_t affected;
        char32_t ignorable;

        [[nodiscard]]
        std::optional<char32_t> get(char32_t codepoint) const
        {
            assert(codepoint - segment < 32);
            auto mask = char32_t(1) << (codepoint - segment);
            if (mask & affected)
                return codepoint + offset;
            if (mask & ignorable)
                return codepoint;
            return {};
        }
    };

    struct CharMapping
    {
        char32_t from_codepoint;
        char32_t to_codepoint;
    };
}
