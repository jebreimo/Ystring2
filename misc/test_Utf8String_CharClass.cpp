//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-05-09.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "../tests/YstringTest/Utf8Chars.hpp"
#include "Utf8String_CharClass.hpp"
#include <catch2/catch.hpp>

using namespace Ystring;

TEST_CASE("Test countCharacters")
{
    REQUIRE(countCharacters("") == 0);
    REQUIRE(countCharacters("A" UTF8_COMBINING_RING_ABOVE "BCDE" UTF8_COMBINING_TILDE) == 5);
}
