//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-06-21.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Encoding.hpp"

#include <algorithm>

namespace Ystring
{
    namespace
    {
        constexpr EncodingInfo ENCODING_INFO[] =
            {
                {Encoding::UNKNOWN, "<unknown>", "",
                 0, 0, Endianness::UNKNOWN},
                {Encoding::UTF_8, "UTF-8", "\xEF\xBB\xBF",
                 1, 4, Endianness::UNKNOWN},
                {Encoding::UTF_16_LE, "UTF-16LE", "\xFF\xFE",
                 2, 2, Endianness::BIG},
                {Encoding::UTF_16_BE, "UTF-16BE", "\xFE\xFF",
                 2, 2, Endianness::BIG},
                {Encoding::UTF_32_LE, "UTF-32LE", "\xFF\xFE\x00\x00",
                 4, 1, Endianness::BIG},
                {Encoding::UTF_32_BE, "UTF-32BE", "\x00\x00\xFE\xFF",
                 4, 1, Endianness::BIG},
                {Encoding::ASCII, "ASCII", "",
                 1, 1, Endianness::UNKNOWN},
            #ifdef YSTRING_ENABLE_ISO_CHARS
                {Encoding::ISO_8859_1, "ISO-8859-1", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::ISO_8859_2, "ISO-8859-2", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::ISO_8859_3, "ISO-8859-3", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::ISO_8859_4, "ISO-8859-4", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::ISO_8859_5, "ISO-8859-5", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::ISO_8859_6, "ISO-8859-6", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::ISO_8859_7, "ISO-8859-7", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::ISO_8859_8, "ISO-8859-8", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::ISO_8859_9, "ISO-8859-9", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::ISO_8859_10, "ISO-8859-10", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::ISO_8859_11, "ISO-8859-11", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::ISO_8859_13, "ISO-8859-13", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::ISO_8859_14, "ISO-8859-14", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::ISO_8859_15, "ISO-8859-15", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::ISO_8859_16, "ISO-8859-16", "",
                 1, 1, Endianness::UNKNOWN},
            #endif
            #ifdef YSTRING_ENABLE_MAC_CHARS
                {Encoding::MAC_CYRILLIC, "MAC-CYRILLIC", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::MAC_GREEK, "MAC-GREEK", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::MAC_ICELAND, "MAC-ICELAND", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::MAC_LATIN2, "MAC-LATIN2", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::MAC_ROMAN, "MAC-ROMAN", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::MAC_TURKISH, "MAC-TURKISH", "",
                 1, 1, Endianness::UNKNOWN},
            #endif
            #ifdef YSTRING_ENABLE_DOS_CHARS
                {Encoding::DOS_CP437, "DOS-CP437", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::DOS_CP737, "DOS-CP737", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::DOS_CP775, "DOS-CP775", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::DOS_CP850, "DOS-CP850", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::DOS_CP852, "DOS-CP852", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::DOS_CP855, "DOS-CP855", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::DOS_CP857, "DOS-CP857", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::DOS_CP860, "DOS-CP860", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::DOS_CP861, "DOS-CP861", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::DOS_CP862, "DOS-CP862", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::DOS_CP863, "DOS-CP863", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::DOS_CP864, "DOS-CP864", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::DOS_CP865, "DOS-CP865", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::DOS_CP866, "DOS-CP866", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::DOS_CP869, "DOS-CP869", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::DOS_CP874, "DOS-CP874", "",
                 1, 1, Endianness::UNKNOWN},
            #endif
            #ifdef YSTRING_ENABLE_WIN_CHARS
                {Encoding::WIN_CP1250, "WIN-CP1250", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::WIN_CP1251, "WIN-CP1251", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::WIN_CP1252, "WIN-CP1252", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::WIN_CP1253, "WIN-CP1253", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::WIN_CP1254, "WIN-CP1254", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::WIN_CP1255, "WIN-CP1255", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::WIN_CP1256, "WIN-CP1256", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::WIN_CP1257, "WIN-CP1257", "",
                 1, 1, Endianness::UNKNOWN},
                {Encoding::WIN_CP1258, "WIN-CP1258", "",
                 1, 1, Endianness::UNKNOWN},
            #endif
            };
    }

    const EncodingInfo& getEncodingInfo(Encoding encoding)
    {
        using std::begin;
        using std::end;
        auto match = std::lower_bound(
            begin(ENCODING_INFO), end(ENCODING_INFO),
            EncodingInfo{encoding},
            [](auto& a, auto& b){return a.encoding < b.encoding;});
        if (match != end(ENCODING_INFO))
            return *match;
        return ENCODING_INFO[0];
    }

    std::pair<const EncodingInfo*, size_t> getSupportedEncodings()
    {
        return {&ENCODING_INFO[1], std::size(ENCODING_INFO) - 1};
    }
}
