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

TEST_CASE("getCharClass on compressed double range with upper case letters")
{
    // At one point there was a bug where value 0 meant both "no value"
    // and upper case in double ranges.
    // This test checks that that bug isn't re-introduced.
    REQUIRE(Ystring::getCharClass(0xC0) == Ystring::CharClass::UPPERCASE_LETTER);
    REQUIRE(Ystring::getCharClass(0xD6) == Ystring::CharClass::UPPERCASE_LETTER);
    REQUIRE(Ystring::getCharClass(0xD7) == Ystring::CharClass::MATH_SYMBOL);
}

TEST_CASE("getCharClass on ASCII character")
{
    REQUIRE(Ystring::getCharClass('A') == Ystring::CharClass::UPPERCASE_LETTER);
}
