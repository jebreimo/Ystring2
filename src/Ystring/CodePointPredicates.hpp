//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-05-08.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

namespace Ystring
{
    // Combining Diacritical Marks (0300–036F), since version 1.0, with modifications in subsequent versions down to 4.1
    constexpr bool isMark(char32_t c)
    {
        return (0x300 <= c && c <= 0x36F)
               || (0x1AB0 <= c && c <= 0x1AFF)
               || (0x1DC0 <= c && c <= 0x1DFF)
               || (0x20D0 <= c && c <= 0x20FF)
               || (0xFE20 <= c && c <= 0xFE2F);
    }
}
