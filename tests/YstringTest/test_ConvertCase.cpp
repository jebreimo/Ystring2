//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-07-30.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/ConvertCase.hpp"
#include <catch2/catch.hpp>

using namespace Ystring;

TEST_CASE("Test ASCII chars upper case")
{
    REQUIRE(toUpper('a') == 'A');
    REQUIRE(toUpper('Z') == 'Z');
    REQUIRE(toUpper(' ') == ' ');
}

TEST_CASE("Test ASCII chars lower case")
{
    REQUIRE(toLower('a') == 'a');
    REQUIRE(toLower('Z') == 'z');
    REQUIRE(toLower(' ') == ' ');
}

TEST_CASE("Test Latin letters")
{
    REQUIRE(toLower(U'ã') == U'ã');
    REQUIRE(toLower(U'Ã') == U'ã');
    REQUIRE(toUpper(U'ã') == U'Ã');
    REQUIRE(toUpper(U'Ã') == U'Ã');
    REQUIRE(toTitle(U'ã') == U'Ã');
    REQUIRE(toTitle(U'Ã') == U'Ã');
}

TEST_CASE("Test toLower")
{
    REQUIRE(toLower("AbcD ÆØÅ.") == "abcd æøå.");
}

TEST_CASE("Test toTitle")
{
    REQUIRE(toTitle("ABCD æøå.") == "Abcd Æøå.");
    REQUIRE(toTitle("ABCD æøå ßß.") == "Abcd Æøå Ssß.");
}

TEST_CASE("Test toUpper")
{
    REQUIRE(toUpper("AbCD æøå.") == "ABCD ÆØÅ.");
    REQUIRE(toUpper("Daß.") == "DASS.");
}
