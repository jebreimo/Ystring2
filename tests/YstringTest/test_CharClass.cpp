//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-07-29.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/CharClass.hpp"
#include <catch2/catch.hpp>

TEST_CASE("getCharClass on compressed double ranges")
{
    REQUIRE(Ystring::getCharClass(0x1E943) == Ystring::CharClass::LOWERCASE_LETTER);
    REQUIRE(Ystring::getCharClass(0x1E944) == Ystring::CharClass::NONSPACING_MARK);
    REQUIRE(Ystring::getCharClass(0x1E94A) == Ystring::CharClass::NONSPACING_MARK);
    REQUIRE(Ystring::getCharClass(0x1E94B) == Ystring::CharClass::MODIFIER_LETTER);
}

TEST_CASE("getCharClass on compressed alternating ranges")
{
    REQUIRE(Ystring::getCharClass(0x1E0D) == Ystring::CharClass::LOWERCASE_LETTER);
    REQUIRE(Ystring::getCharClass(0x1E0E) == Ystring::CharClass::UPPERCASE_LETTER);
}
