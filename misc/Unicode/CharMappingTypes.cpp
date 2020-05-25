//****************************************************************************
// Copyright © 2013 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2013-07-01
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "CharMappingTypes.hpp"

#include <cassert>

namespace Ystring { namespace Unicode
{
    bool CompactCharMapping::get(char32_t chr, char32_t& mappedChr) const
    {
        assert(chr - segment < 32);
        auto mask = char32_t(1) << (chr - segment);
        if (mask & affected)
        {
            mappedChr =  chr + offset;
            return true;
        }
        else if (mask & ignorable)
        {
            mappedChr = chr;
            return true;
        }
        return false;
    }
}}
