//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-05-05.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Encodings/EncodeUtf8.hpp"

#include <catch2/catch.hpp>

using namespace Ystring;

namespace
{
    void testEncodeArray(char32_t c, std::string_view expected)
    {
        CAPTURE(c, expected);
        char s[4] = {};
        REQUIRE(encodeUtf8(s, sizeof(s), c) == expected.size());
        REQUIRE(std::string_view(s, expected.size()) == expected);
    }
}

TEST_CASE("Test encodeUtf8")
{
    testEncodeArray(0, std::string_view("\0", 1));
    testEncodeArray('A', "A");
    testEncodeArray(0x7F, "\x7F");
    testEncodeArray(0x80, "\xC2\x80");
    testEncodeArray(0x7FF, "\xDF\xBF");
    testEncodeArray(0x800, "\xE0\xA0\x80");
    testEncodeArray(0x800, "\xE0\xA0\x80");
    testEncodeArray(0xFFFF, "\xEF\xBF\xBF");
    testEncodeArray(0x10000, "\xF0\x90\x80\x80");
    testEncodeArray(0x1FFFFF, "\xF7\xBF\xBF\xBF");
    testEncodeArray(0x200000, {});
}
