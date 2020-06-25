//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-06-21.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Encoding.hpp"

namespace Ystring
{
    namespace
    {
        constexpr EncodingInfo ENCODING_INFO[] =
            {
                {Encoding::UNKNOWN, "<unknown>", "",
                 0, 0, Endianness::UNKNOWN},
                {Encoding::UTF_8, "UTF-8", "\xEF\xBB\xBF",
                 1, 4, Endianness::UNKNOWN}
            };
    }

    const EncodingInfo& getEncodingInfo(Encoding encoding)
    {
        if (size_t(encoding) < sizeof(ENCODING_INFO) / sizeof(*ENCODING_INFO))
            return ENCODING_INFO[size_t(encoding)];
        return ENCODING_INFO[0];
    }
}
