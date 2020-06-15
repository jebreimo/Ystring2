//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-06-01.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Endian.hpp"

namespace Ystring
{
    enum class Encoding
    {
        UTF8 = 0,
        UTF16_LE = 1,
        UTF16_BE = 2,
        UTF16_NATIVE = IsLittleEndian ? UTF16_LE : UTF16_BE,
        UTF32_LE = 3,
        UTF32_BE = 4,
        UTF32_NATIVE = IsLittleEndian ? UTF32_LE : UTF32_BE,
        ASCII = 10,
        ISO8859_1 = 11,
    };
}
