//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-07-30.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/ConvertCase.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace ystring;

TEST_CASE("Test ASCII chars upper case")
{
    REQUIRE(to_upper('a') == 'A');
    REQUIRE(to_upper('Z') == 'Z');
    REQUIRE(to_upper(' ') == ' ');
}

TEST_CASE("Test ASCII chars lower case")
{
    REQUIRE(to_lower('a') == 'a');
    REQUIRE(to_lower('Z') == 'z');
    REQUIRE(to_lower(' ') == ' ');
}

TEST_CASE("Test Latin letters")
{
    REQUIRE(to_lower(U'ã') == U'ã');
    REQUIRE(to_lower(U'Ã') == U'ã');
    REQUIRE(to_upper(U'ã') == U'Ã');
    REQUIRE(to_upper(U'Ã') == U'Ã');
    REQUIRE(to_title(U'ã') == U'Ã');
    REQUIRE(to_title(U'Ã') == U'Ã');
}

TEST_CASE("Test to_lower")
{
    REQUIRE(to_lower("AbcD ÆØÅ.") == "abcd æøå.");
}

TEST_CASE("Test to_title")
{
    REQUIRE(to_title("ABCD æøå.") == "Abcd Æøå.");
    REQUIRE(to_title("ABCD æøå ßß.") == "Abcd Æøå Ssß.");
}

TEST_CASE("Test to_upper")
{
    REQUIRE(to_upper("AbCD æøå.") == "ABCD ÆØÅ.");
    REQUIRE(to_upper("Daß.") == "DASS.");
}
