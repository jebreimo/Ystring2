//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-08-17.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Ystring/Utf32.hpp"
#include <catch2/catch.hpp>

using namespace ystring;

TEST_CASE("Test from_utf32")
{
    REQUIRE(from_utf32(U"AÅéæΩ") == u8"AÅéæΩ");
}

TEST_CASE("Test to_utf32")
{
    REQUIRE(to_utf32(u8"AÅéæΩ") == U"AÅéæΩ");
}
