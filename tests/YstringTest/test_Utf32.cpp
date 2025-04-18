//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-08-17.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Utf32.hpp"
#include <catch2/catch_test_macros.hpp>
#include "U8Adapter.hpp"

using namespace ystring;

TEST_CASE("Test from_utf32 char32_t")
{
    REQUIRE(from_utf32(U'Å') == U8("Å"));
}

TEST_CASE("Test from_utf32 u32string")
{
    REQUIRE(from_utf32(U"AÅéæΩ") == U8("AÅéæΩ"));
}

TEST_CASE("Test to_utf32")
{
    REQUIRE(to_utf32(U8("AÅéæΩ")) == U"AÅéæΩ");
}
