//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-07-29.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/CharClass.hpp"
#include <catch2/catch.hpp>

TEST_CASE("get_char_class on compressed double ranges")
{
    REQUIRE(ystring::get_char_class(0x1E943) == ystring::CharClass::LOWERCASE_LETTER);
    REQUIRE(ystring::get_char_class(0x1E944) == ystring::CharClass::NONSPACING_MARK);
    REQUIRE(ystring::get_char_class(0x1E94A) == ystring::CharClass::NONSPACING_MARK);
    REQUIRE(ystring::get_char_class(0x1E94B) == ystring::CharClass::MODIFIER_LETTER);
}

TEST_CASE("get_char_class on compressed alternating ranges")
{
    REQUIRE(ystring::get_char_class(0x1E0D) == ystring::CharClass::LOWERCASE_LETTER);
    REQUIRE(ystring::get_char_class(0x1E0E) == ystring::CharClass::UPPERCASE_LETTER);
}

TEST_CASE("get_char_class on compressed double range with upper case letters")
{
    // At one point there was a bug where value 0 meant both "no value"
    // and upper case in double ranges.
    // This test checks that that bug isn't re-introduced.
    REQUIRE(ystring::get_char_class(0xC0) == ystring::CharClass::UPPERCASE_LETTER);
    REQUIRE(ystring::get_char_class(0xD6) == ystring::CharClass::UPPERCASE_LETTER);
    REQUIRE(ystring::get_char_class(0xD7) == ystring::CharClass::MATH_SYMBOL);
}

TEST_CASE("get_char_class on ASCII character")
{
    REQUIRE(ystring::get_char_class('A') == ystring::CharClass::UPPERCASE_LETTER);
}
