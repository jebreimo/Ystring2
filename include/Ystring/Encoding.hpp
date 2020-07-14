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
#include "YstringDefinitions.hpp"

namespace Ystring
{
    enum class Encoding
    {
        UNKNOWN = 0,
        UTF_8,
        UTF_16_LE,
        UTF_16_BE,
        UTF_32_LE,
        UTF_32_BE,
        ASCII,
        UTF_16_NATIVE = IsLittleEndian ? UTF_16_LE : UTF_16_BE,
        UTF_32_NATIVE = IsLittleEndian ? UTF_32_LE : UTF_32_BE,
    #ifdef YSTRING_ENABLE_ISO_CHARS
        ISO_8859_1 = 128,
        ISO_8859_2,
        ISO_8859_3,
        ISO_8859_4,
        ISO_8859_5,
        ISO_8859_6,
        ISO_8859_7,
        ISO_8859_8,
        ISO_8859_9,
        ISO_8859_10,
        ISO_8859_11,
        ISO_8859_13,
        ISO_8859_14,
        ISO_8859_15,
        ISO_8859_16,
    #endif
    #ifdef YSTRING_ENABLE_MAC_CHARS
        MAC_CYRILLIC = 256,
        MAC_GREEK,
        MAC_ICELAND,
        MAC_LATIN2,
        MAC_ROMAN,
        MAC_TURKISH,
    #endif
    #ifdef YSTRING_ENABLE_DOS_CHARS
        DOS_CP437 = 512,
        DOS_CP737,
        DOS_CP775,
        DOS_CP850,
        DOS_CP852,
        DOS_CP855,
        DOS_CP857,
        DOS_CP860,
        DOS_CP861,
        DOS_CP862,
        DOS_CP863,
        DOS_CP864,
        DOS_CP865,
        DOS_CP866,
        DOS_CP869,
        DOS_CP874,
    #endif
    #ifdef YSTRING_ENABLE_WIN_CHARS
        WIN_CP1250 = 1024,
        WIN_CP1251,
        WIN_CP1252,
        WIN_CP1253,
        WIN_CP1254,
        WIN_CP1255,
        WIN_CP1256,
        WIN_CP1257,
        WIN_CP1258,
    #endif
    };

    struct EncodingInfo
    {
        Encoding encoding;
        std::string_view name = {};
        std::string_view byteOrderMark = {};
        size_t unitSize = 0;
        size_t maxUnits = 0;
        Endianness endianness = Endianness::UNKNOWN;
    };

    [[nodiscard]]
    const EncodingInfo& getEncodingInfo(Encoding encoding);

    [[nodiscard]]
    std::pair<const EncodingInfo*, size_t> getSupportedEncodings();
}
