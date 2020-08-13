//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-07-30.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/ConvertCase.hpp"
#include <catch2/catch.hpp>

TEST_CASE("Test ASCII chars upper case")
{
    REQUIRE(Ystring::toUpper('a') == 'A');
    REQUIRE(Ystring::toUpper('Z') == 'Z');
    REQUIRE(Ystring::toUpper(' ') == ' ');
}

TEST_CASE("Test ASCII chars lower case")
{
    REQUIRE(Ystring::toLower('a') == 'a');
    REQUIRE(Ystring::toLower('Z') == 'z');
    REQUIRE(Ystring::toLower(' ') == ' ');
}

TEST_CASE("Test Latin letters")
{
    REQUIRE(Ystring::toLower(U'ã') == U'ã');
    REQUIRE(Ystring::toLower(U'Ã') == U'ã');
    REQUIRE(Ystring::toUpper(U'ã') == U'Ã');
    REQUIRE(Ystring::toUpper(U'Ã') == U'Ã');
    REQUIRE(Ystring::toTitle(U'ã') == U'Ã');
    REQUIRE(Ystring::toTitle(U'Ã') == U'Ã');
}
