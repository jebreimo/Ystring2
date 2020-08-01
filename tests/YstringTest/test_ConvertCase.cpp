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
    REQUIRE(Ystring::upper('a') == 'A');
    REQUIRE(Ystring::upper('Z') == 'Z');
    REQUIRE(Ystring::upper(' ') == ' ');
}

TEST_CASE("Test ASCII chars lower case")
{
    REQUIRE(Ystring::lower('a') == 'a');
    REQUIRE(Ystring::lower('Z') == 'z');
    REQUIRE(Ystring::lower(' ') == ' ');
}

TEST_CASE("Test Latin letters")
{
    REQUIRE(Ystring::lower(U'ã') == U'ã');
    REQUIRE(Ystring::lower(U'Ã') == U'ã');
    REQUIRE(Ystring::upper(U'ã') == U'Ã');
    REQUIRE(Ystring::upper(U'Ã') == U'Ã');
    REQUIRE(Ystring::title(U'ã') == U'Ã');
    REQUIRE(Ystring::title(U'Ã') == U'Ã');
}
