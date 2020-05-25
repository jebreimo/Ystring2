//****************************************************************************
// Copyright © 2013 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2013-07-01
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <cstdint>
#include <utility>
#include "Ystring/YstringDefinitions.hpp"

namespace Ystring { namespace Unicode
{
    struct YSTRING_API CompactCharMapping
    {
        char32_t segment;
        char32_t offset;
        char32_t affected;
        char32_t ignorable;

        bool get(char32_t chr, char32_t& mappedChr) const;
    };

    struct CharMapping
    {
        char32_t chr;
        char32_t mappedChr;
    };
}}
