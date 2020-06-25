//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-06-01.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <string>
#include "Endian.hpp"

namespace Ystring
{
    enum class Encoding
    {
        UNKNOWN = 0,
        UTF_8,
        UTF_16_LE,
        UTF_16_BE,
        UTF_16_NATIVE = IsLittleEndian ? UTF_16_LE : UTF_16_BE,
        UTF_32_LE,
        UTF_32_BE,
        UTF_32_NATIVE = IsLittleEndian ? UTF_32_LE : UTF_32_BE,
        ASCII,
        ISO_8859_1,
        WIN_CP_1252
    };

    struct EncodingInfo
    {
        Encoding encoding;
        std::string_view name;
        std::string_view byteOrderMark;
        size_t unitSize = 0;
        size_t maxUnits = 0;
        Endianness endianness = Endianness::UNKNOWN;
    };

    const EncodingInfo& getEncodingInfo(Encoding encoding);
}
