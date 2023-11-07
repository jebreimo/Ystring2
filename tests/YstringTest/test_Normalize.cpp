//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-08-16.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Normalize.hpp"
#include <catch2/catch_test_macros.hpp>
#include "U8Adapter.hpp"

using namespace ystring;

TEST_CASE("Test normalize")
{
    REQUIRE(normalize("Abcd 123") == "Abcd 123");
    REQUIRE(normalize(U8("Ä")) == U8("A\u0308"));
    REQUIRE(normalize(U8("\u01D5")) == U8("U\u0308\u0304"));
    REQUIRE(normalize(U8("CD\u01D5qr")) == U8("CDU\u0308\u0304qr"));
}

TEST_CASE("Test denormalize")
{
    REQUIRE(denormalize("Abcd 123") == "Abcd 123");
    REQUIRE(denormalize(U8("A\u0308")) == U8("Ä"));
}

TEST_CASE("Denormalize, two marks, chars before and after.")
{
    REQUIRE(denormalize(U8("CDU\u0308\u0304qr")) == U8("CD\u01D5qr"));
}
